/*
 * hrprof - High Resolution Profiling Library
 * Copyright (C) 2001 Pace Willisson <pace@alum.mit.edu>
 * Released under the GPL: see the file COPYING
 *
 * $Id: hrprof.c,v 1.1 2010/01/14 19:12:12 wb6ymh Exp $
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/gmon.h>
#include <sys/gmon_out.h>

#define rdtscll(val) __asm__ __volatile__ ("rdtsc;" : "=A" (val))

extern void _start (void), etext (void);
extern int main (int argc, char **argv);

static int hrprof_trace;

static void hrprof_finish (void);

static double processor_clock;

static int hrprof_state;

static int hrprof_scale_ready;
static double hrprof_scale;
static double profile_rate;

void
hrprof_set_scale (int code)
{
   if (code == 1000) {
      hrprof_scale = 1000;
      profile_rate = 1000;
   } else {
      hrprof_scale = 1;
      profile_rate = 1000000;
   }
   hrprof_scale_ready = 1;
}

static unsigned long long last_tsc;
static unsigned long long start_tsc;

static double start_time;

static unsigned long long ticks_given_out;

static void nop () {}
#define N8 nop();nop();nop();nop();nop();nop();nop();nop();
#define N64 N8 N8 N8 N8 N8 N8 N8 N8

static void hrprof_lib_placeholder (void) {N64}
static void hrprof_stray_enter (void) {N64}
static void hrprof_exit_from_stray (void) {N64}
static void hrprof_arc_not_found (void) {N64}
static void hrprof_sentinel (void) {N64}

struct func;
static struct func *find_func (void *fn_addr, int create);

static struct func *func_stray_enter;
static struct func *func_exit_from_stray;
static struct func *func_arc_not_found;
static struct func *func_library_placeholder;


static unsigned long long
get_ticks (void)
{
   unsigned long long tsc, ticks;

   rdtscll (tsc);
   ticks = tsc - last_tsc;
   last_tsc = tsc;

   ticks_given_out += ticks;

   return (ticks);
}

static double
get_time (void)
{
   struct timeval tv;
   gettimeofday (&tv, NULL);
   return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

struct sym {
   char *name;
   void *addr;
};

#define NSYMS 10000
static struct sym syms[NSYMS];
int sym_count;

static int
sym_cmp (void const *raw1, void const *raw2)
{
   struct sym const *arg1 = (struct sym const *)raw1;
   struct sym const *arg2 = (struct sym const *)raw2;

   if (arg1->addr < arg2->addr)
      return (-1);
   if (arg1->addr > arg2->addr)
      return (1);
   return (0);
}

char *
hrprof_sym (char *buf, void *addr)
{
   int l, u, m;
   struct sym *best;
   struct sym *sp;

   l = 0;
   u = sym_count - 1;

   best = NULL;
   while (l <= u) {
      m = (l + u) / 2;
      sp = &syms[m];
      if (sp->addr < addr) {
         if (best == NULL
             || (sp->addr > best->addr))
            best = sp;
         l = m + 1;
      } else if (sp->addr > addr) {
         u = m - 1;
      } else {
         best = sp;
         break;
      }
   }

   if (best == NULL)
      sprintf (buf, "%p", addr);
   else if (best->addr == addr)
      strcpy (buf, best->name);
   else
      sprintf (buf, "%s+%d", best->name, addr - best->addr);

   return (buf);
}

void
hrprof_init_syms (void)
{
   char cmd[1000]; /* XXX */
   FILE *f;
   char buf[1000];
   unsigned long ul_addr;
   char name[1000];
   struct sym *sp;
   char symtype;

   sprintf (cmd, "nm -p /proc/%d/exe", getpid ());
   if ((f = popen (cmd, "r")) == NULL)
      return;

   while (fgets (buf, sizeof buf, f) != NULL) {
      if (sscanf (buf, "%lx %c %s", &ul_addr, &symtype, name) != 3)
         continue;
      if (symtype != 'T' && symtype != 't')
         continue;
      sp = &syms[sym_count++];
      if ((sp->name = strdup (name)) == NULL) {
         fprintf (stderr, "out of memory\n");
         exit (1);
      }
      sp->addr = (void *)ul_addr;
      
      if (sym_count >= NSYMS)
         break;
   }

   pclose (f);

   qsort (syms, sym_count, sizeof *syms, sym_cmp);
}

static void
hrprof_init (void *this_fn, void *call_site)
{
   double test_secs;
   unsigned long long test_ticks;
   int pass;

   hrprof_init_syms ();

   atexit (hrprof_finish);

   test_secs = .050;
   test_ticks = 0;
   for (pass = 0; pass < 3; pass++) {
      unsigned long long before, after, ticks;
      rdtscll (before);
      start_time = get_time ();
      while (get_time () - start_time < test_secs)
         ;
      rdtscll (after);
      ticks = after - before;
      if (test_ticks == 0 || ticks < test_ticks)
         test_ticks = ticks;
   }

   processor_clock = test_ticks / test_secs;

   func_stray_enter = find_func (hrprof_stray_enter, 1);
   func_exit_from_stray = find_func (hrprof_exit_from_stray, 1);
   func_arc_not_found = find_func (hrprof_arc_not_found, 1);
   func_library_placeholder = find_func (hrprof_lib_placeholder, 1);

   start_time = get_time ();
   rdtscll (start_tsc);
   last_tsc = start_tsc;

   hrprof_state = 1;
}

#define NHASH 10007

struct arc {
   struct arc *hash_next;
   void *fn_addr;
   void *call_site;
   struct func *from_func;
   struct func *to_func;
   int count;
   int callback_flag;
};

static struct arc *arc_hash[NHASH];
static struct arc *cur_arc;

struct func {
   struct func *next;
   void *fn_addr;
   unsigned long long ticks;
};

static struct func *funcs;
static struct func *cur_func;

struct returns {
   struct returns *next;
   void *call_site;
};

static struct returns *returns_hash[NHASH];

static int
find_return (void *call_site, int create)
{
   unsigned int h;
   struct returns *rp;

   h = (unsigned int)call_site % NHASH;

   for (rp = returns_hash[h]; rp; rp = rp->next)
      if (rp->call_site == call_site)
         return (1);

   if (create == 0)
      return (0);

   if ((rp = calloc (1, sizeof *rp)) == NULL) {
      fprintf (stderr, "out of memory\n");
      exit (1);
   }

   rp->call_site = call_site;
   
   rp->next = returns_hash[h];
   returns_hash[h] = rp;

   return (1);
}

static struct func *
find_closest_func (void *call_site)
{
   struct func *best_fp, *fp;

   if (call_site >= (void *)hrprof_sentinel) {
      if (hrprof_trace)
         printf ("func too high %p\n", call_site);
      return (NULL);
   }

   best_fp = NULL;
   for (fp = funcs; fp; fp = fp->next) {
      if (fp->fn_addr > call_site)
         continue;
      if (best_fp == NULL || fp->fn_addr > best_fp->fn_addr)
         best_fp = fp;
   }

   return (best_fp);
}

static struct func *
find_func (void *fn_addr, int create)
{
   struct func *fp;

   if (fn_addr >= (void *)hrprof_sentinel) {
      fprintf (stderr, "hrprof: bad link order\n");
      exit (1);
   }

   for (fp = funcs; fp; fp = fp->next)
      if (fp->fn_addr == fn_addr)
         return (fp);

   if (create == 0)
      return (NULL);

   if ((fp = calloc (1, sizeof *fp)) == NULL) {
      fprintf (stderr, "out of memory\n");
      exit (1);
   }
   fp->fn_addr = fn_addr;
   
   fp->next = funcs;
   funcs = fp;

   return (fp);
}

static struct arc *
find_arc (void *fn_addr, void *call_site, int create)
{
   struct arc *cp;
   int h;

   h = (unsigned int)call_site % NHASH;
   for (cp = arc_hash[h]; cp; cp = cp->hash_next) {
      if (cp->fn_addr == fn_addr
          && cp->call_site == call_site)
         return (cp);
   }

   if (create == 0)
      return (NULL);

   if ((cp = calloc (1, sizeof *cp)) == NULL) {
      fprintf (stderr, "out of memory\n");
      exit (1);
   }
   
   cp->call_site = call_site;
   cp->fn_addr = fn_addr;
   
   cp->from_func = find_closest_func (call_site);
   cp->to_func = find_func (fn_addr, 1);
   
   cp->hash_next = arc_hash[h];
   arc_hash[h] = cp;
      
   return (cp);
}

/*
 * this_fn is start address of called function
 * call_site is return address
 */
void
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
   struct arc *cp;
   unsigned long long ticks;
   char namebuf[1000];

   if (hrprof_state == 0)
      hrprof_init (this_fn, call_site);

   if (hrprof_trace) {
      char from[1000], to[1000], cur[1000], ret[1000];
      printf ("\n%-20s calls %-20s (cur %s ret %s)\n",
         hrprof_sym (from, call_site),
         hrprof_sym (to, this_fn),
         cur_func
         ? hrprof_sym (cur, cur_func->fn_addr)
         : "-none-",
         hrprof_sym (ret, __builtin_return_address(1)));
   }

   ticks = get_ticks ();

   cp = find_arc (this_fn, call_site, 1);
   if (cur_func) {
#if 1
      if(strcmp(hrprof_sym(namebuf,cur_func->fn_addr),"DeltaTime2String") == 0) {
         printf("WTF?\n");
      }
#endif
      if (cur_func == cp->from_func) {
         if (hrprof_trace)
            printf ("  normal enter (charge %s)\n",
               hrprof_sym(namebuf,cur_func->fn_addr));
         cur_func->ticks += ticks;
         find_return (call_site, 1);
      } else if (cur_func == func_library_placeholder) {
         if (hrprof_trace)
            printf ("  normal enter after"
               " library_placeholder"
               " (charge library_placeholder)\n");
         func_library_placeholder->ticks += ticks;
      } else if (find_return (call_site, 0)) {
         if (hrprof_trace)
            printf ("  enter inline (charge %s)\n",
               hrprof_sym(namebuf,cur_func->fn_addr));
         cur_func->ticks += ticks;
         goto skip;
      } else {
         /* entering a callback from a library */
         if (hrprof_trace)
            printf ("  enter callback"
               " (charge library_placeholder)\n");
         cp->callback_flag = 1;
         func_library_placeholder->ticks += ticks;
      }
   } else if (this_fn == main) {
      if (hrprof_trace)
         printf ("  program start\n");
      cp->callback_flag = 1;
   } else {
      if (hrprof_trace)
         printf ("  stray enter (charge stray_enter)\n");
      cp->callback_flag = 1;
      func_stray_enter->ticks += ticks;
   }

   cp->count++;
   cur_func = cp->to_func;
   cur_arc = cp;

 skip:;
}

void
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
   struct arc *cp;
   unsigned long long ticks, ticks1, ticks2;
   char namebuf[1000];

   if (hrprof_state == 0)
      hrprof_init (NULL, NULL);

   if (hrprof_trace) {
      char from[1000], to[1000], cur[1000];
      printf ("\n%-20s retto %-20s (cur %s)\n",
         hrprof_sym (to, this_fn),
         hrprof_sym (from, call_site),
         cur_func
         ? hrprof_sym (cur, cur_func->fn_addr)
         : "-none-");
   }

   ticks = get_ticks ();

   cp = find_arc (this_fn, call_site, 0);

   if (cp != NULL) {
      if (cur_func == cp->to_func) {
         if (hrprof_trace)
            printf ("  normal return (charge %s)\n",
               hrprof_sym(namebuf,cur_func->fn_addr));
         cur_func->ticks += ticks;
      } else if (cur_func == func_library_placeholder) {
         if (hrprof_trace)
            printf ("  normal return after having"
               " been in library"
               " charge lib placeholder and %s\n",
               hrprof_sym (namebuf,
                      cp->to_func->fn_addr));
         ticks1 = ticks / 2;
         ticks2 = ticks - ticks1;
         func_library_placeholder->ticks += ticks1;
         cp->to_func->ticks += ticks2;
      } else if (cur_func == NULL) {
         if (hrprof_trace)
            printf ("  exit from stray"
               " (charge exit stray)\n");
         func_exit_from_stray->ticks += ticks;
      } else if (find_return (call_site, 0)) {
         if (hrprof_trace)
            printf ("  exit from inline"
               " (charge %s)\n",
               hrprof_sym (namebuf,
                      cur_func->fn_addr));
         cur_func->ticks += ticks;
         goto skip;
      } else {
         if (hrprof_trace)
            printf ("  surprise return to %s\n",
               hrprof_sym (namebuf,
                      cur_func->fn_addr));
      }

      if (cp->callback_flag) {
         if (hrprof_trace)
            printf ("  returning to library\n");
         cur_func = func_library_placeholder;
      } else {
         if (hrprof_trace && cp->from_func == NULL)
            printf ("strange ... from_func is NULL\n");
         cur_func = cp->from_func;
      }
   } else {
      if (hrprof_trace)
         printf ("  hrprof exit: arc not found\n");
      func_arc_not_found->ticks += ticks;
      cur_func = NULL;
   }

   cur_arc = cp;

 skip:;
}

static HISTCOUNTER *kcount;
static unsigned int kcountsize;
static unsigned int kmax_idx;
static unsigned long lowpc, highpc;

static unsigned long long
build_hist (void)
{
   unsigned long textsize;
   int max_bucket_val;
   struct func *fp;
   struct func *Hog = NULL;
   unsigned int togo;
   unsigned int idx;
   double ticks;
   double secs;
   char namebuf[1000];
   unsigned long long MaxTicks = 0;

   lowpc = (unsigned long)_start;
   highpc = (unsigned long)etext;

   lowpc = ROUNDDOWN(lowpc, HISTFRACTION * sizeof(HISTCOUNTER));
   highpc = ROUNDUP(highpc, HISTFRACTION * sizeof(HISTCOUNTER));
   textsize = highpc - lowpc;
   kcountsize = textsize / HISTFRACTION;
   kmax_idx = kcountsize / sizeof (HISTCOUNTER);

   kcount = calloc (1, kcountsize);

   max_bucket_val = 0xffff;

   ticks = 0;

// sh: set the scale based on the function that used the most time
   for (fp = funcs; fp; fp = fp->next) {
      if(MaxTicks < fp->ticks) {
         MaxTicks = fp->ticks;
         Hog = fp;
      }
   }

   if (hrprof_trace)
      printf("Time hog is %s\n",hrprof_sym(namebuf,Hog->fn_addr));

   secs = MaxTicks / processor_clock * hrprof_scale;
   profile_rate = max_bucket_val / secs;

   for (fp = funcs; fp; fp = fp->next) {
      if (fp->ticks == 0)
         continue;
      ticks += fp->ticks;
      secs = fp->ticks / processor_clock * hrprof_scale;
      togo = secs * profile_rate + .5;
      idx = ((unsigned int)fp->fn_addr - lowpc)
         / HISTFRACTION / sizeof (HISTCOUNTER);
      if (hrprof_trace)
         printf ("record %g secs (%u) for %s idx %d (%d)\n",
            secs, togo,
            hrprof_sym (namebuf, fp->fn_addr),
            idx, togo);
      if (togo < 1)
         togo = 1;
      else if(togo > max_bucket_val) {
         togo = max_bucket_val;
      }
      kcount[idx] = togo;
   }

   return (ticks);
}

static void
put4 (void *dest, unsigned int val)
{
   memcpy (dest, &val, sizeof (unsigned int));
}

static void
hrprof_finish (void)
{
   struct gmon_hdr ghdr;
   struct gmon_hist_hdr thdr;
   struct gmon_cg_arc_record raw_arc;
   int h;
   struct arc *cp;
   double tsc_secs;
   double tod_secs;
   double charged_secs;
   double end_time;
   unsigned long long charged_ticks;
   unsigned long long used_ticks;
   char from_name[1000], to_name[1000];
   int idx;

   FILE *f;

   if (hrprof_state != 1)
      return;
   hrprof_state = 2;

   end_time = get_time ();
   tod_secs = end_time - start_time;

   if (hrprof_scale_ready == 0) {
      if (tod_secs < 10)
         hrprof_set_scale (1000);
      else
         hrprof_set_scale (1);
   }

   charged_ticks = build_hist ();

   used_ticks = last_tsc - start_tsc;

   charged_secs = charged_ticks / processor_clock;

   tsc_secs = used_ticks / processor_clock;

   remove ("hrprof.out");
   if ((f = fopen ("hrprof.out", "w")) != NULL) {
      fprintf (f, "processor clock %g MHz\n",
          processor_clock / 1000000.0);
      fprintf (f, "secs according to tsc: %g\n", tsc_secs);
      fprintf (f, "secs according to tod: %g\n", tod_secs);
      fprintf (f, "charged secs %g\n", charged_secs);
      fprintf (f, "given out %g\n",
          ticks_given_out / processor_clock);

      fprintf (f, "hist:\n");
      for (idx = 0; idx < kmax_idx; idx++) {
         if (kcount[idx]) {
            void *pc;
            pc = (void *)(idx * HISTFRACTION
                     * sizeof (HISTCOUNTER)
                     + lowpc);
            fprintf (f, "%s %d\n",
                hrprof_sym (from_name, pc),
                kcount[idx]);
         }
      }

      fclose (f);
   }

   remove ("gmon.out");
   if ((f = fopen ("gmon.out", "w")) == NULL)
      return;

   memset (&ghdr, 0, sizeof ghdr);
   memcpy (ghdr.cookie, GMON_MAGIC, sizeof ghdr.cookie);
   put4 (ghdr.version, GMON_VERSION);
   fwrite (&ghdr, 1, sizeof ghdr, f);

   memset (&thdr, 0, sizeof thdr);
   put4 (thdr.low_pc, lowpc);
   put4 (thdr.high_pc, highpc);
   put4 (thdr.hist_size, kcountsize / sizeof (HISTCOUNTER));
   put4 (thdr.prof_rate, profile_rate);
   if (hrprof_scale == 1) {
      strcpy (thdr.dimen, "secs");
      thdr.dimen_abbrev = 's';
   } else {
      strcpy (thdr.dimen, "msecs");
      thdr.dimen_abbrev = 'u';
   }

   putc (GMON_TAG_TIME_HIST, f);
   fwrite (&thdr, 1, sizeof thdr, f);
   fwrite (kcount, 1, kcountsize, f);

   for (h = 0; h < NHASH; h++) {
      for (cp = arc_hash[h]; cp; cp = cp->hash_next) {
         if (hrprof_trace)
            printf ("arc: %s %s %d\n",
               hrprof_sym (from_name, cp->call_site),
               hrprof_sym (to_name, cp->fn_addr),
               cp->count);
         put4 (raw_arc.from_pc,
               (unsigned int)cp->call_site);
         put4 (raw_arc.self_pc,
               (unsigned int)cp->fn_addr);
         put4 (raw_arc.count, cp->count);

         putc (GMON_TAG_CG_ARC, f);
         fwrite (&raw_arc, 1, sizeof raw_arc, f);
      }
   }

   fclose (f);

   free (kcount);
}
