/* Copyright (C) 2002 GFRN systems

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.

   The latest version of this program may be found at
   http://CQiNet.sourceforge.net

   $Log: configfile.c,v $
   Revision 1.17  2010/11/17 04:46:26  wb6ymh
   Set ConfigPass in ParseConfigLine. Programmer is too lazy to pass it
   as an argument.  Get over it, this is just a "hobby".

   Revision 1.16  2009/09/13 23:16:53  wb6ymh
   Modified ParseConfigLine to return an error on unknown configuration
   variables for the Manual pass only.

   Revision 1.15  2009/06/30 23:23:15  wb6ymh
   Modified parse_configfile to continue parsing after an error.

   Revision 1.14  2008/05/14 18:11:32  wb6ymh
   1. Added code to strip trailing spaces from end of string configuration
      file variables to some difficult to find problems.  Thanks to N4KLB for
      the suggestion.

   Revision 1.13  2008/04/27 15:31:08  wb6ymh
   Modified read_config to prevent checking that required class configuration
   varialbles are set.  They are checked elsewhere and are only required if
   the class was created which may be optional.

   Revision 1.12  2008/03/21 14:46:53  wb6ymh
   Modified ParseConfigLine to accept assigments without spaces around the '='.

   Revision 1.11  2008/02/26 18:02:08  wb6ymh
   Added dmalloc support.

   Revision 1.10  2008/01/13 16:52:18  wb6ymh
   Added support for CON_FLG_ALL_PASSES.

   Revision 1.9  2007/12/14 22:38:26  wb6ymh
   1. Changed calling convention of SetFunc and changed it's name to AccessFunc
      to better reflect current usage.
   2. Deleted GetBitValue and SetBitMap.

   Revision 1.8  2007/12/01 00:57:09  wb6ymh
   Added GetBitValue and SetBitMap.

   Revision 1.7  2007/11/26 06:55:48  wb6ymh
   Added support for include files.

   Revision 1.6  2007/07/02 13:33:44  wb6ymh
   Fixed bug in ParseConfigLine: variables that are not active in the current
   pass incorrectly returned an error.

   Revision 1.5  2007/06/12 23:34:17  wb6ymh
   1. Added support for MANUAL_SET and CON_FLG_ONCE_ONLY to ParseConfigLine.
   2. Modified ParseConfigLine to actually return error codes on error.

   Revision 1.4  2004/05/29 17:46:02  wb6ymh
   1. Modified configuration file parser to accept both DOS and *nix end of line
      conventions (*nix version only, Windoze version has always accepted both).

   Revision 1.3  2003/08/31 23:00:11  wb6ymh
   Modified read_config() to take an already opened file handle rather than
   a filename so the config file can be kept open between passes.

   Revision 1.2  2003/08/16 13:24:00  wb6ymh
   Added support for multiple configuration passes.

   Revision 1.1.1.1  2002/11/02 17:03:54  wb6ymh
   Moved common source files to common subdirectory from src.

   Revision 1.2  2002/09/14 16:45:50  wb6ymh
   Added return error codes to setfunctions.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "configfile.h"

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

// Use a non-case sensitive compare function if one is available
#ifdef HAVE_STRICMP
   #define STRCMP stricmp
#else
#ifdef HAVE_STRCASECMP
   #define STRCMP strcasecmp
#else
   #define STRCMP strcmp
#endif
#endif

/*
   This routine is a general configuration file parser.
   The format of the configuration file is:
   variable=string;
   A structure describing the configuration variables and
   their type is passed to this routne.  since sscanf() is used to convert
   the value, all standard types are supported.  Use "%[^\n]" to input
   strings with embeded spaces.

   Ret = 0 on success, error code on failure
*/
int ParseConfigLine(char *line,struct config_entry *config_vars,int Pass)
{
   int Ret = 0;   // Assume the best
   struct config_entry *p = config_vars;
   char var_name[121];
   char var_value[121];
   char var_type_temp[4];
   int  var_temp;
   int   i = 0;
   char  *cp;
   void *var_ptr;

   ConfigPass = Pass;
   if((cp = strrchr(line,'\n')) != NULL) {
      // remove terminating <lf>
      *cp = 0;
   }

   if((cp = strrchr(line,'\r')) != NULL) {
   // remove terminating <cr> just in case some one uses an editor that
   // changes the end of line convention in *nix
      *cp = 0;
   }

   cp = &line[strlen(line)-1];
// Strip any trailing whitespace
   while(cp >= line && isspace(*cp)) {
       *cp-- = 0;
   }

   if(is_comment(line)) {
   // ignore this line if it's a comment
   }
   else if(sscanf(line," %[^= ]%*[= ]%[^\n]",var_name,var_value) != 2) {
   // line didn't convert
      LOG_WARN(("Error: Unable to parse \"%s\".\n       Line ignored.\n",
                line));
   }
   else for(i = 0; p->var_name[0] != 0; p++, i++) {
      if(!STRCMP(var_name,p->var_name)) {
         // found our guy
         if(!(p->Flags & CON_FLG_ALL_PASSES) &&
            (p->Flags & CON_FLG_IGNORE || 
            (Pass != MANUAL_SET && Pass != CON_FLG_PASS(p->Flags))))
         {  // Ignore this guy
            break;
         }

         if(!(p->Flags & CON_FLG_ALL_PASSES) && p->Flags & CON_FLG_SET) {
            // already set
            if(p->Flags & CON_FLG_ONCE_ONLY) {
               LOG_WARN(("Error: Variable \"%s\" can only be set once, keeping "
                         "earlier value.\n",p->var_name));
               break;
            }
            else if(Pass != MANUAL_SET) {
               LOG_WARN(("Warning: Variable \"%s\" already set, ignoring "
                         "earlier value.\n",p->var_name));
            }
         }

         if(p->Flags & CON_FLG_BASED_VAR) {
            if((var_ptr = ConfigGetVarPtr(p)) == NULL) {
               Ret = 1;
               break;
            }
         }
         else {
            var_ptr = p->var_ptr;
         }

         if(p->AccessFunc != NULL) {
         // Variable has a set function, call it
            Ret = p->AccessFunc(p,var_value,AF_SET_VAR,0);
         }
         else if(p->var_type[0] == '%' && p->var_type[1] == 's' ) {
         // special case, string value
            *((char * *) var_ptr) = strdup(var_value);
            p->Flags |= CON_FLG_SET;
         }
         else if(p->var_type[0] == '%' && p->var_type[1] == 'b' ) {
         // special case, byte value
            var_type_temp[0] = '%';
            var_type_temp[1] = p->var_type[2];
            var_type_temp[2] = 0;
            if(sscanf(var_value,var_type_temp,&var_temp) != 1){
               line[strlen(line)-1] = 0;  // remove terminating <cr>
               LOG_WARN(("Error: Unable to convert \"%s\".\n",line));
               Ret = 1;
            }
            else if(var_temp > 255 || var_temp < 0) {
               line[strlen(line)-1] = 0;  // remove terminating <cr>
               LOG_ERROR(("Error: Byte value out of range \"%s\".\n",line));
               Ret = 1;
            }
            else {
               *((char *) var_ptr) = (char) var_temp;
               p->Flags |= CON_FLG_SET;
            }
         }
         else {
            if(sscanf(var_value,p->var_type,var_ptr) != 1) {
               line[strlen(line)-1] = 0;  // remove terminating <cr>
               LOG_ERROR(("Error: Unable to convert \"%s\".\n",line));
               Ret = 1;
            }
            else{
               p->Flags |= CON_FLG_SET;
            }
         }
         break;
      }
   }

   if(config_vars[i].var_name[0] == 0) {
      if(Pass == MANUAL_SET) {
         Ret = 1;
      }
      else {
         LOG_ERROR(("Error: Unknown configuration variable \"%s\", ignored.\n",
                    var_name));
      }
   }
   return Ret;
}

int parse_configfile(FILE *config,struct config_entry *config_vars,int Pass)
{
   #define MAX_INCLUDES 8
   static int Nested = 0;
   int Ret = 0;
   int Return = 0;
   FILE *fp = NULL;
   char *cp;
   char *cp1;
   char line[121];

   while(fgets(line,120,config) != NULL) {
      if(!is_comment(line)) {
         if(strncmp(line,"include ",8) == 0) {
         // include another file here
            if(++Nested >= MAX_INCLUDES) {
               LOG_ERROR(("Error: include file nested is too deep.\n"));
               Ret = CONFIG_NESTING_ERROR;
               break;
            }
            cp = &line[8];
            while(*cp && (*cp == ' ' || *cp == '\t' || *cp == '"')) {
               cp++;
            }

            if((cp1 = strchr(cp,'\n')) != NULL) {
               *cp1 = 0;
            }

            if((cp1 = strchr(cp,'"')) != NULL) {
               *cp1 = 0;
            }

            if((fp = fopen(cp,"r")) == NULL) {
               LOG_ERROR(("Error: couldn't open \"%s\" - %s.\n",
                          cp,strerror(errno)));
               Ret = CONFIG_OPEN_ERROR;
               break;
            }
         // Process the included file
            Ret = parse_configfile(fp,config_vars,Pass);
            Nested--;
         }
         else {
            Ret = ParseConfigLine(line,config_vars,Pass);
         }
      }
      if(Ret != 0 && Return == 0) {
      // Return the error code for the first error encountered and press on
         Return = Ret;
      }
   }

   if(fp != NULL) {
      fclose(fp);
   }
   return Return;
}

int read_config(FILE *config,struct config_entry *config_vars,int Pass)
{
   int i;
   int Ret = 0;

   // clear the 'set' variable of all possible configuration variables

   for(i = 0; config_vars[i].var_name[0] != 0; i++) {
      if(CON_FLG_PASS(config_vars[i].Flags) == Pass &&
         !(config_vars[i].Flags & CON_FLG_ONCE_ONLY))
      {
         config_vars[i].Flags &= ~CON_FLG_SET;
      }
   }

   Ret = parse_configfile(config,config_vars,Pass);

   // now check that all of the the required varables for this pass were set

   if(Ret == 0) {
      for(i = 0; config_vars[i].var_name[0] != 0; i++){
         if((config_vars[i].Flags & CON_FLG_REQUIRED) && 
            CON_FLG_PASS(config_vars[i].Flags) == Pass &&
            !(config_vars[i].Flags & CON_FLG_BASED_VAR) &&
            !(config_vars[i].Flags & CON_FLG_SET))
         {
            LOG_ERROR(("Error: Required configuration variable \"%s\""
                       " is not set.\n",
                  config_vars[i].var_name));
            Ret = CONFIG_REQUIRED_MISSING;
            break;
         }
      }
   }
   return Ret;
}

int is_comment(char *cp)
{
   while(*cp && isspace(*cp))
      cp++;
   if(*cp == ';' || *cp == '#' || *cp == 0 )
      return 1;
   return 0;
}


