/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
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
 *
 * $Id: logout.c,v 1.1 1997/09/21 05:59:42 gdr Exp $
 *
 * This file is formatted for tab stops every 8 columns.
 */

#ifdef __ORCAC__
segment "libutil___";
#endif

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)logout.c	8.1 (Berkeley) 6/4/93";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/time.h>

#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct utmp UTMP;

#ifdef  TESTRUN
#undef  _PATH_UTMP
#undef  _PATH_WTMP
#define _PATH_UTMP "/tmp/utmp"
#define _PATH_WTMP "/tmp/wtmp"
#endif

int
logout(register char *line)
{
	register int fd;
	UTMP ut;
	int rval;

#ifdef __ORCAC__
	if ((fd = open(_PATH_UTMP, O_RDWR)) < 0)
#else
	if ((fd = open(_PATH_UTMP, O_RDWR, 0)) < 0)
#endif
		return(0);
	rval = 0;
	while (read(fd, &ut, sizeof(UTMP)) == sizeof(UTMP)) {
		if (!ut.ut_name[0] || strncmp(ut.ut_line, line, UT_LINESIZE))
			continue;
		bzero(ut.ut_name, UT_NAMESIZE);
		bzero(ut.ut_host, UT_HOSTSIZE);
		(void)time(&ut.ut_time);
		(void)lseek(fd, -(off_t)sizeof(UTMP), L_INCR);
		(void)write(fd, &ut, sizeof(UTMP));
		rval = 1;
	}
	(void)close(fd);
	return(rval);
}
