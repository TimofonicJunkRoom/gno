/*-
 * Copyright (c) 1980 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __GNO__
#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)main.c  5.16 (Berkeley) 3/27/91";
#endif /* not lint */
#endif

#define USE_OLD_TTY

#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <sgtty.h>
#include <time.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <paths.h>
#include <libutil.h>
#include <err.h>
#include <gno/gno.h>
#include "gettytab.h"

static int	getname (void);
static void	putpad (char *s);
static int	puts (char *s);
static void	putchr (int cc);
static void	oflush (void);
static void	prompt (void);
static void	putf (char *cp);

struct  sgttyb tmode = {
    0, 0, CERASE, CKILL, 0
};
struct  tchars tc = {
    CINTR, CQUIT, CSTART,
    CSTOP, CEOF, CBRK,
};
struct  ltchars ltc = {
    CSUSP, CDSUSP, CRPRNT,
    CFLUSH, CWERASE, CLNEXT
};

int crmod, digit, lower, upper;

char    hostname[MAXHOSTNAMELEN];
char    name[16];
char    dev[] = _PATH_DEV;
char    ttyn[32];

#ifdef __ORCAC__
char *ttyname(int fino);
#else
char    *ttyname();
#endif

#define OBUFSIZ     128
#define TABBUFSIZ   512

char    defent[TABBUFSIZ];
char    defstrs[TABBUFSIZ];
char    tabent[TABBUFSIZ];
char    tabstrs[TABBUFSIZ];

#ifndef __ORCAC__
char    *env[128];
#endif

char partab[] = {
    0001,0201,0201,0001,0201,0001,0001,0201,
    0202,0004,0003,0205,0005,0206,0201,0001,
    0201,0001,0001,0201,0001,0201,0201,0001,
    0001,0201,0201,0001,0201,0001,0001,0201,
    0200,0000,0000,0200,0000,0200,0200,0000,
    0000,0200,0200,0000,0200,0000,0000,0200,
    0000,0200,0200,0000,0200,0000,0000,0200,
    0200,0000,0000,0200,0000,0200,0200,0000,
    0200,0000,0000,0200,0000,0200,0200,0000,
    0000,0200,0200,0000,0200,0000,0000,0200,
    0000,0200,0200,0000,0200,0000,0000,0200,
    0200,0000,0000,0200,0000,0200,0200,0000,
    0000,0200,0200,0000,0200,0000,0000,0200,
    0200,0000,0000,0200,0000,0200,0200,0000,
    0200,0000,0000,0200,0000,0200,0200,0000,
    0000,0200,0200,0000,0200,0000,0000,0201
};

#define ERASE   tmode.sg_erase
#define KILL    tmode.sg_kill
#define EOT tc.t_eofc

jmp_buf timeout;

static void
dingdong(int sig, int code)
{

    alarm(0);
    signal(SIGALRM, SIG_DFL);
    longjmp(timeout, 1);
}

jmp_buf intrupt;

static void
interrupt(int sig, int code)
{

    signal(SIGINT, interrupt);
    longjmp(intrupt, 1);
}

int
main(int argc, char **argv)
{
#ifndef __ORCAC__
    extern  char **environ;
#endif
    char *tname;
    long allflags;
    int repcnt = 0;
    int tmp;

    __REPORT_STACK();

    
    signal(SIGINT, SIG_IGN);
/*
    signal(SIGQUIT, SIG_DFL);
*/
    openlog("getty", LOG_ODELAY|LOG_CONS, LOG_AUTH);
    gethostname(hostname, sizeof(hostname));
    if (hostname[0] == '\0')
        strcpy(hostname, "Amnesiac");
    
    /*
     * The following is a work around for vhangup interactions
     * which cause great problems getting window systems started.
     * If the tty line is "-", we do the old style getty presuming
     * that the file descriptors are already set up for us. 
     * J. Gettys - MIT Project Athena.
     */
    if (argc <= 2 || strcmp(argv[2], "-") == 0) {
    char *temp = ttyname(STDIN_FILENO);
        if (temp) strcpy(ttyn, temp); else *ttyn = 0;
    } else {
        int i;

#ifdef __ORCAC__
	strcpy(ttyn,argv[2]);
#else
        strcpy(ttyn, dev);
        strncat(ttyn, argv[2], sizeof(ttyn)-sizeof(dev));
#endif
        if (strcmp(argv[0], "+") != 0) {
/*        chown(ttyn, 0, 0);    */
/*        chmod(ttyn, 0600);    */
/*        revoke(ttyn);         */
        /*
         * Delay the open so DTR stays down long enough to be detected.
         */
/*        sleep(2);       */
        while ((i = open(ttyn, O_RDWR)) == -1) {
            if (repcnt % 10 == 0) {
                syslog(LOG_ERR, "%s: %m", ttyn);
                closelog();
            }
            repcnt++;
            sleep(60);
        }
        login_tty(i); /* ??? */
        }
    }
    gettable("default", defent, defstrs);
    gendefaults();
    tname = "default";
    if (argc > 1)
        tname = argv[1];
    for (;;) {
        int ldisp = OTTYDISC;
        int off = 0;

        gettable(tname, tabent, tabstrs);
        if (OPset || EPset || APset)
            APset++, OPset++, EPset++;
        setdefaults();
        ioctl(STDIN_FILENO, TIOCFLUSH, 0);     /* clear out the crap */
        ioctl(STDIN_FILENO, FIONBIO, &off);    /* turn off non-blocking mode */
        ioctl(STDIN_FILENO, FIOASYNC, &off);   /* ditto for async mode */
        if (IS)
            tmode.sg_ispeed = speed(IS);
        else if (SP)
            tmode.sg_ispeed = speed(SP);
        if (OS)
            tmode.sg_ospeed = speed(OS);
        else if (SP)
            tmode.sg_ospeed = speed(SP);
        tmode.sg_flags = setflags(0);
        ioctl(STDIN_FILENO, TIOCSETP, &tmode);
        setchars();
        ioctl(STDIN_FILENO, TIOCSETC, &tc);
        if (HUset)
	    ioctl(STDIN_FILENO, TIOCSHUP, &HU);
        if (HC)
            ioctl(STDIN_FILENO, TIOCHPCL, 0);
        if (AB) {
            tname = autobaud();
            continue;
        }
        if (PS) {
            tname = portselector();
            continue;
        }
        if (CL && *CL)
            putpad(CL);
        edithost(HE);
        if (IM && *IM) 
            putf(IM);
        if (setjmp(timeout)) {
            tmode.sg_ispeed = tmode.sg_ospeed = 0;
            ioctl(STDIN_FILENO, TIOCSETP, &tmode);
            exit(1);
        }
        if (TO) {
            signal(SIGALRM, dingdong);
            alarm(TO);
        }
        tmp = getname();
        /*printf("\r\n\t\t\ttmp: %d\n\r",tmp);*/
        fflush(stdout);
        if (tmp) {
        /*if (getname()) {*/
#ifndef __ORCAC__
            register int i;
#endif

            oflush();
            alarm(0);
            signal(SIGALRM, SIG_DFL);
            if (name[0] == '-') {
                puts("user names may not start with '-'.");
                continue;
            }
            if (!(upper || lower || digit))
                continue;
            allflags = setflags(2);
            tmode.sg_flags = allflags & 0xffff;
            allflags >>= 16;
            if (crmod || NL)
                tmode.sg_flags |= CRMOD;
            if (upper || UC)
                tmode.sg_flags |= LCASE;
            if (lower || LC)
                tmode.sg_flags &= ~LCASE;
            tmode.sg_erase = 0x7f;
            ioctl(STDIN_FILENO, TIOCSETP, &tmode);
            ioctl(STDIN_FILENO, TIOCSLTC, &ltc);
            ioctl(STDIN_FILENO, TIOCLSET, &allflags);
            signal(SIGINT, SIG_DFL);
#ifdef __ORCAC__
            BUG("making environment");
            makeenv();
            BUG("made environment");
#else
            for (i = 0; environ[i] != (char *)0; i++)
                env[i] = environ[i];
            makeenv(&env[i]);
#endif

            /* 
             * this is what login was doing anyway.
             * soon we rewrite getty completely.
             */
            set_ttydefaults(STDIN_FILENO);
#ifdef __ORCAC__
            {
            static char temp[256];
                sprintf(temp,"login -p %s",name);
#ifdef __STACK_CHECK__
		/*
		 * Because we're doing the _execve, we won't get it from
		 * the atexit(3) registered function.
		 */
		warnx("stack usage: %d bytes", _endStackCheck());
#endif
                _execve(LO,temp);
            }
#else
            execle(LO, "login", "-p", name, (char *) 0, env);
#endif
            syslog(LOG_ERR, "%s: %m", LO);
            exit(1);
        }
        alarm(0);
        signal(SIGALRM, SIG_DFL);
        signal(SIGINT, SIG_IGN);
        if (NX && *NX)
            tname = NX;
    }
}

static int
getname(void)
{
    register int c;
    register char *np;
    char cs;

    /*
     * Interrupt may happen if we use CBREAK mode
     */
    if (setjmp(intrupt)) {
        signal(SIGINT, SIG_IGN);
        return (0);
    }
    signal(SIGINT, interrupt);
    tmode.sg_flags = setflags(0);
    ioctl(STDIN_FILENO, TIOCSETP, &tmode);
    tmode.sg_flags = setflags(1);
    /*printf("flags: %04X\n",tmode.sg_flags);*/
    prompt();
    if (PF > 0) {
        oflush();
        sleep(PF);
        PF = 0;
    }
    ioctl(STDIN_FILENO, TIOCSETP, &tmode);
    crmod = digit = lower = upper = 0;
    np = name;
    for (;;) {
        oflush();
        if (read(STDIN_FILENO, &cs, 1) <= 0)
            exit(0);
        if ((c = cs&0177) == 0)
            return (0);
        if (c == EOT)
            exit(1);
        if (c == '\r' || c == '\n' || np >= &name[sizeof name]) {
            putf("\r\n");
            break;
        }
        if (islower(c))
            lower = 1;
        else if (isupper(c))
            upper = 1;
        else if (c == ERASE || c == '#' || c == '\b' || c == 0x7f) {
            if (np > name) {
                np--;
                if (tmode.sg_ospeed >= B1200)
                    puts("\b \b");
                else
                    putchr(cs);
            }
            continue;
        } else if (c == KILL || c == '@') {
            putchr(cs);
            putchr('\r');
            if (tmode.sg_ospeed < B1200)
                putchr('\n');
            /* this is the way they do it down under ... */
            else if (np > name)
                puts("                                     \r");
            prompt();
            np = name;
            continue;
        } else if (isdigit(c))
            digit++;
        if (IG && (c <= ' ' || c > 0176))
            continue;
        *np++ = c;
        putchr(cs);
    }
    signal(SIGINT, SIG_IGN);
    *np = 0;
    if (c == '\r')
        crmod = 1;
    if (upper && !lower && !LC || UC)
        for (np = name; *np; np++)
            if (isupper(*np))
                *np = tolower(*np);
    return (1);
}

static
short   tmspc10[] = {
    0, 2000, 1333, 909, 743, 666, 500, 333, 166, 83, 55, 41, 20, 10, 5, 15
};

static void
putpad(char *s)
{
    register pad = 0;
    register mspc10;

    if (isdigit(*s)) {
        while (isdigit(*s)) {
            pad *= 10;
            pad += *s++ - '0';
        }
        pad *= 10;
        if (*s == '.' && isdigit(s[1])) {
            pad += s[1] - '0';
            s += 2;
        }
    }

    puts(s);
    /*
     * If no delay needed, or output speed is
     * not comprehensible, then don't try to delay.
     */
    if (pad == 0)
        return;
    if (tmode.sg_ospeed <= 0 ||
        tmode.sg_ospeed >= (sizeof tmspc10 / sizeof tmspc10[0]))
        return;

    /*
     * Round up by a half a character frame, and then do the delay.
     * Too bad there are no user program accessible programmed delays.
     * Transmitting pad characters slows many terminals down and also
     * loads the system.
     */
    mspc10 = tmspc10[tmode.sg_ospeed];
    pad += mspc10 / 2;
    for (pad /= mspc10; pad > 0; pad--)
        putchr(*PC);
}

/* odd, another prototyping bug */
int
puts(char *s)
    /*register char *s;*/
{
    while (*s)
        putchr(*s++);
    return 0;
}

char    outbuf[OBUFSIZ];
int obufcnt = 0;

static void
putchr(int cc)
{
    char c;

    c = cc;
    if (!NP) {
        c |= partab[c&0177] & 0200;
        if (OP)
            c ^= 0200;
    }
    if (!UB) {
        outbuf[obufcnt++] = c;
        if (obufcnt >= OBUFSIZ)
            oflush();
    } else
        write(STDOUT_FILENO, &c, 1);
}

static void
oflush(void)
{
    if (obufcnt)
        write(STDOUT_FILENO, outbuf, obufcnt);
    obufcnt = 0;
}

static void
prompt(void)
{

    putf(LM);
    if (CO)
        putchr('\n');
}

static void
putf(char *cp)
{
    extern char editedhost[];
    time_t t;
    char *slash;
    static char db[100];

    while (*cp) {
        if (*cp != '%') {
            putchr(*cp++);
            continue;
        }
        switch (*++cp) {

        case 't':
            slash = rindex(ttyn, '/');
            if (slash == NULL)
                puts(ttyn);
            else
                puts(&slash[1]);
            break;

        case 'h':
            puts(editedhost);
            break;

        case 'd': {
#ifdef NO_STRFTIME
	    (void)time(&t);
	    puts(ctime(&t));
#else	/* NO_STRFTIME */
#ifdef __GNO__
	    /*
	     * We don't use SCCS for the GNO sources, so we don't have
	     * to worry about a 'M' after a '%' character.  As far as
	     * %P is concerned, it looks like it was just plain wrong.
	     */
	    static char fmt[] = "%l:%M%p on %A, %d %B %Y";
#else	/* __GNO__ */
            static char fmt[] = "%l:% %P on %A, %d %B %Y";

            fmt[4] = 'M';       /* I *hate* SCCS... */
#endif	/* __GNO__ */
            (void)time(&t);
            (void)strftime(db, sizeof(db), fmt, localtime(&t));
            puts(db);
#endif	/* NO_STRFTIME */
            break;
        }

        case '%':
            putchr('%');
            break;
        }
        cp++;
    }
}
