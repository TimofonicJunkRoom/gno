/*
 * mkso is used for maintaining manual page ".so links" for the GNO
 * base distribution.  See the manual page for details.
 *
 * $Id: mkso.c,v 1.2 1997/12/21 22:39:13 gdr Exp $
 *
 * Devin Reade, 1997.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#ifdef __GNO__
#include <gno/gno.h>
#endif

#define BUFFERSIZE 1024
#define DELIM " \t"

static int isProdosFileName(const char *name);

char *progname = NULL;
int deleteFiles = 0;
int verbose = 0;

void
usage (void) {
  printf("This program is part of the GNO installation package.\n");
  printf("It creates .so (nroff source files) for the man package\n\n");
  printf("Usage: %s [-dhv] [-H dir] datafile\n", progname);
  printf("\t-H dir\tspecify alternate directory for non-ProDOS links\n");
  printf("\t-h\tprint usage information\n");
  printf("\t-d\tdelete instead of create files (safe)\n");
  printf("\t-v\tverbose operation\n");
  exit(1);
}

#ifdef __STACK_CHECK__
static void printstack(void) {
	fprintf(stderr, "Stack Usage: %d bytes\n", _endStackCheck());
}
#endif

int main(int argc, char **argv) {
  static char dataBuffer[BUFFERSIZE];
  static char magicBuffer[BUFFERSIZE];
  static char hfsdir[PATH_MAX];
  static char cwd[PATH_MAX];
  static char *magic = ".\\\" This file is auto-generated by mkso.\n";
  FILE *fp, *outfp;
  char *p, *file, *new, *org, *hfsptr;
  int line = 0;
  int H_flag = 0;
  int c;
  int useHFSdir, hfsdirlen, sawColon, sawSlash;

#ifdef __STACK_CHECK__
  _beginStackCheck();
  atexit(printstack);
#endif
#ifdef __GNO__
  _setPathMapping(1);
#endif
  progname = argv[0];
  hfsdir[0] = '\0';
  hfsptr = hfsdir;
  while ((c = getopt(argc, argv, "dhvH:")) != EOF) {
    switch (c) {
    case 'd':
      deleteFiles = 1;
      break;

    case 'H':
      if (H_flag) {
        fprintf(stderr, "-H flag can only be specified once.  Second and "
                "subsequent invocations ignored\n");
        break;
      }
      H_flag++;
      hfsdirlen = strlen(optarg);
      if (hfsdirlen > PATH_MAX-2) {
        fprintf(stderr, "pathname for -H flag too long\n");
        exit(1);
      }
      p = optarg;
      sawColon = sawSlash = 0;
      while (*p) {
        switch (*p) {
        case ':':
          sawColon=1;
          if (sawSlash) {
            goto die;	/* piss off; I know it's bad form */
          }
          *hfsptr++ = '/';	/* map to '/' */
          break;
        case '/':
          sawSlash=1;
          if (sawColon) {
die:
            fprintf(stderr, "you cannot use pathnames that contain both '/' "
                    "and ':' in this program\n");
            exit(1);
          }
          /*FALLTHROUGH*/
        default:
          *hfsptr++ = *p++;
        }
      }
      *hfsptr++ = '/';
      hfsdirlen++;
      if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("couldn't determine current directory");
        exit(1);
      }
      break;

    case 'v':
      verbose = 1;
      break;

    case 'h':
    default:
      usage();
    }
  }
  if (argc - optind != 1) {
    usage();
  } else {
    file = argv[optind];
  }

  if ((fp = fopen(file, "r")) == NULL) {
    perror("couldn't open data file");
    exit(1);
  }

  while (fgets(dataBuffer, BUFFERSIZE, fp) != NULL) {
    line++;

    /* eliminate comments and newlines -- get file names */
    if ((p = strchr(dataBuffer, '#')) != NULL ||
	(p = strchr(dataBuffer, '\n')) != NULL) {
      *p = '\0';
    }
    if ((org = strtok(dataBuffer, DELIM)) == NULL) {
      continue;
    }
    if ((new = strtok(NULL, DELIM)) == NULL) {
      fprintf(stderr, "missing new file name at line %d of %s", line, file);
      continue;
    }
    if (!H_flag || isProdosFileName(new)) {
      useHFSdir = 0;
    } else {
      int len;

      len = strlen(new);
      if (len + hfsdirlen + 1 > PATH_MAX-1) {
        fprintf(stderr, "pathname for %s too long\n", new);
        exit(1);
      }
      strcpy(hfsptr, new);
      new = hfsdir;
      useHFSdir = 1;
    }

    if (deleteFiles) {
      if ((outfp = fopen(new, "r")) == NULL) {
	/* file doesn't exist -- doesn't need deleting */
	continue;
      }
      
      /* look for magic string on second line */
      if (fgets(magicBuffer, BUFFERSIZE, outfp) == NULL ||
	  fgets(magicBuffer, BUFFERSIZE, outfp) == NULL ||
	  strcmp(magicBuffer, magic)) {
	printf("bad magic for %s -- not deleted\n", new);
      } else {
	if (verbose) {
	  printf("rm -f %s\n", new);
	}
	unlink(new);
      }
    } else {

      if (access(new, F_OK) == 0) {
	if (verbose) {
	  printf("file %s already exists -- skipping\n", new);
	}
	continue;
      }

      if (verbose) {
	printf("linking %s to %s\n", new, org);
      }

    
      if ((outfp = fopen(new, "w")) == NULL) {
	fprintf(stderr,
		"couldn't open \"%s\" from line %d: %s: file skipped\n",
		new, line, strerror(errno));
	continue;
      }
      if (useHFSdir) {
        fprintf(outfp, ".so %s/%s\n", cwd, org);
      } else {
        fprintf(outfp, ".so %s\n", org);
      }
      fprintf(outfp, magic);
      fclose(outfp);
    }
  }

  if (ferror(fp)) {
    perror("error on reading data file");
    exit(1);
  }

  fclose(fp);
  return 0;
}

/*
 * returns 1 if <name> follows ProDOS conventions, zero otherwise.
 */

static int
isProdosFileName (const char *name)
{
  const char *p;
  int len, c;

  p = basename(name);

  /* special case the first character */
  c = tolower(*p);
  if (! ((c >= 'a') && (c <= 'z'))) {
    return 0;
  }
  p++;
  len = 1;

  while(*p) {
    len++;
    if (len > 15) {
      return 0;
    }
    c = *p;
    if (isalnum(c) || c == '.') {
	p++;
    } else {
        return 0;
    }
  }
  return 1;
}
