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

   $Log: configfile.h,v $
   Revision 1.10  2010/11/17 04:44:48  wb6ymh
   Added ConfigPass.

   Revision 1.9  2008/05/14 18:13:48  wb6ymh
   Changed var_name and var_type to const char * to fix compile problems with
   GCC 4.2.x.

   Revision 1.8  2008/01/13 16:52:52  wb6ymh
   Added CON_FLG_ALL_PASSES.

   Revision 1.7  2007/12/15 00:53:52  wb6ymh
   Changed SetFunc to AccessFunc.  AccessFunc is extensable, SetFunc was not.

   Revision 1.6  2007/12/01 00:56:26  wb6ymh
   Added SetBitMap prototype.

   Revision 1.5  2007/11/26 06:59:52  wb6ymh
   1. Added extern "C" around prototypes when compiled for C++.
   2. Added CON_FLG_BASED_VAR flag to support configuration variables that
      are class members.
   3. Added ConfigGetVarPtr prototype.

   Revision 1.4  2007/06/12 23:40:10  wb6ymh
   1. Added CON_FLG_MANUAL, MANUAL_SET.
   2. Added prototypes for is_comment and ParaseConfigLine.

   Revision 1.3  2003/08/31 23:10:23  wb6ymh
   Modified read_config() to take an already opened file handle rather than
   a filename so the config file can be kept open between passes.

   Revision 1.2  2003/08/16 13:48:20  wb6ymh
   1. Modified config_entry to support multiple pass configuration.
   2. Added Pass argument to read_config() prototype.

   Revision 1.1.1.1  2002/10/06 17:00:22  wb6ymh
   Move common header files to inc subdirectory from src.

   Revision 1.2  2002/09/14 16:46:42  wb6ymh
   Added return error codes to set functions.

   Revision 1.1.1.1  2002/08/10 20:33:41  wb6ymh
   initial import

*/

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

typedef enum {
   AF_SET_VAR = 1,
   AF_SET_DEFAULT,
   AF_DISPLAY_VAR,
} AfCmd;

struct config_entry {
   const char *var_name;   // variable name
   const char *var_type;   // variable type
   void *var_ptr;          // pointer to variable
   int   Flags;
      #define CON_FLG_PASS1      0x0   // prior to conference initializaton
      #define CON_FLG_PASS2      0x1   // after initialization
      #define CON_FLG_PASS3      0x2   // after 1st station list is received
      #define CON_FLG_PASS4      0x3   // after 2nd server login succeeds
      #define CON_FLG_MANUAL     0x7   // Set command
      #define CON_FLG_PASS(x)    ((x & 0x7)+1)
      
      #define CON_FLG_REQUIRED   0x8
      #define CON_FLG_SET        0x10
      #define CON_FLG_IGNORE     0x20
      #define CON_FLG_ONCE_ONLY  0x40
      #define CON_FLG_BASED_VAR  0x80   // var_ptr is an offset
      #define CON_FLG_ALL_PASSES 0x100
   int (*AccessFunc)(struct config_entry *p, char *Arg, AfCmd Cmd, int x);
};

#define  MANUAL_SET              0xff


#define  CONFIG_OPEN_ERROR       1  // specified configuration file not found
#define  CONFIG_REQUIRED_MISSING 2  // required variable was not set
#define  BAD_VALUE               3  // variable couldn't be assigned
#define  CONFIG_NESTING_ERROR    4  // nesting of include files is too deep

extern int ConfigPass;

#ifdef __cplusplus
extern "C" {
#endif

void SetConfigDefaults(int bSet);
int read_config(FILE *fp,struct config_entry *config_vars,int Pass);
int ReReadConfig();
int is_comment(char *cp);
int ParseConfigLine(char *line,struct config_entry *config_vars,int Pass);
void *ConfigGetVarPtr(struct config_entry *config_vars);

#ifdef __cplusplus
}
#endif

#endif
