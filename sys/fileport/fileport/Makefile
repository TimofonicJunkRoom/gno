#
# $Id: Makefile,v 1.2 1998/12/13 02:47:36 gdr-ftp Exp $
#
# Devin Reade, December 1997

ODIR	= obj
INCLUDE	= ../includes
CFLAGS	+= -I$(INCLUDE) -w -v
OBJS	= $(ODIR)/port.o $(ODIR)/c.port.o $(ODIR)/errors.o $(ODIR)/errormsgs.o
LDLIBS	= -l ../support/libdvr
HDRS	= $(INCLUDE)/ks.fileio.e $(INCLUDE)/ks.defines.h \
	  $(INCLUDE)/portable.c.h $(INCLUDE)/ks.routines.h \
	  $(INCLUDE)/ks.memory.h
build:	$(ODIR) FilePort

FilePort: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LDLIBS)
	chtyp -t187 -a2 $@
# 187 == 0xBB

$(ODIR):
	install -d $(ODIR)

$(ODIR)/%.o: %.c;	$(CC) -o $@ -c $(CFLAGS) $<
$(ODIR)/%.o: %.asm;	$(CC) -o $@ -c $(CFLAGS) $<
                                   
port.o::	port.macros
c.port.o::	$(HDRS) errors.h
errors.o::	$(INCLUDE)/portable.c.h errors.h
