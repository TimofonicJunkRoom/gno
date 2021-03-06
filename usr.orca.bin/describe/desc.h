/*
 * Header file for the describe package.  This file is
 * used by describe(1), descc(8), and descu(8).
 *
 * $Id: desc.h,v 1.8 1999/04/05 19:47:19 gdr-ftp Exp $
 */

/*
 * Keep these utilities in lockstep. Don't forget to change the version
 * number in the *.rez files and in the manual pages.
 */
#define _VERSION_ "v1.0.7"
#define CURRENT_YEAR "1999"

#define QUOTE_CHAR '#'
#define DATABASE   "/usr/lib/describe.db"

#define FIELD_LEN        9
#define NAME_LEN        36
#define MAX_LINE_LENGTH 81
#define FIELD_COUNT      7 /* number of fields below, not including comments */

#define NAME    "Name:    "
#define VERSION "Version: "
#define SHELL   "Shell:   "
#define AUTHOR  "Author:  "
#define CONTACT "Contact: "
#define WHERE   "Where:   "
#define FTP     "FTP:     "

#define NAME_SHORT    "Name:"
#define VERSION_SHORT "Version:"
#define SHELL_SHORT   "Shell:"
#define AUTHOR_SHORT  "Author:"
#define CONTACT_SHORT "Contact:"
#define WHERE_SHORT   "Where:"
#define FTP_SHORT     "FTP:"

#ifndef   FALSE
#  define FALSE 0
#  define TRUE  1
#endif

typedef short int2;
typedef long  int4;

typedef struct nameEntry_tag {
  char     name[NAME_LEN];
  long int offset; 
} nameEntry;

typedef struct descEntryTag {
  char *name;
  char *data;
} descEntry;

extern int optind;
extern char *optarg;

/* extern int getopt_restart(void); */
extern void begin_stack_check(void);
extern int  end_stack_check(void);
extern char *basename(char *);
