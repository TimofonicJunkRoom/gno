#
#  Makefile for use with dmake on Apple IIGS to create getvers test files
#  $Id: Makefile,v 1.1 1998/04/29 00:42:01 tribby Exp $
#
#  Written by Dave Tribby  *  April 1998

# Default values for parameters
PROD	= product
VERSION	= 1,0,0,release,0
DESC	= Program description
COMMENT	= Program comment

# Longer values for parameters
LPROD  = Super Graphics Destroyer
LVERSION = 2,0,0,release,0
LDESC = \\\\0xA9 1991 Pretty as a Picture, Inc.
LCOMMENT = This is the comment displayed by the Finder\\\\0xAA when file \\\\0x11Information is requested.

# Default rule for creating country rez file
   %.rez : rezskel ; modskel $* ver$* $(VERSION) '$(PROD)' '$(DESC)' '$(COMMENT)'

# Resource files to be created:
GOODOBJ	= US.r Russia.r Yugo.r Ireland.r Thai.r
BADVER	= Arabia.r Britain.r Cyprus.r Denmark.r Latvia.r France.r
LONGER  = Portugal.r
BADCTRY = Bad26.r Bad55.r Bad80.r
DEFAUL1	= Germany.r Italy.r Netherlands.r BelgiumLux.r Sweden.r Spain.r
DEFAUL2	= Australia.r Finland.r FrSwiss.r GrSwiss.r Greece.r Iceland.r Malta.r
DEFAUL3	= India.r Pakistan.r Lithuania.r Poland.r Hungary.r Estonia.r Lapland.r
DEFAUL4	= Iran.r Korea.r China.r Taiwan.r FaeroeIsl.r Turkey.r FrCanada.r
OBJS	= $(GOODOBJ) $(BADVER) $(LONGER) $(BADCTRY) $(DEFAUL1) $(DEFAUL2) $(DEFAUL3) $(DEFAUL4)


# Groups of characters
CHR0	= 00-0F:\\\\0x00\\\\0x01\\\\0x02\\\\0x03\\\\0x04\\\\0x05\\\\0x06\\\\0x07\\\\0x08\\\\0x09\\\\0x0A\\\\0x0B\\\\0x0C\\\\0x0D\\\\0x0E\\\\0x0F
CHR1	= 10-1F:\\\\0x10\\\\0x11\\\\0x12\\\\0x13\\\\0x14\\\\0x15\\\\0x16\\\\0x17\\\\0x18\\\\0x19\\\\0x1A\\\\0x1B\\\\0x1C\\\\0x1D\\\\0x1E\\\\0x1F
CHR2	= 20-2F:\\\\0x20\\\\0x21\\\\0x22\\\\0x23\\\\0x24\\\\0x25\\\\0x26\\\\0x27\\\\0x28\\\\0x29\\\\0x2A\\\\0x2B\\\\0x2C\\\\0x2D\\\\0x2E\\\\0x2F
CHR3	= 30-3F:\\\\0x30\\\\0x31\\\\0x32\\\\0x33\\\\0x34\\\\0x35\\\\0x36\\\\0x37\\\\0x38\\\\0x39\\\\0x3A\\\\0x3B\\\\0x3C\\\\0x3D\\\\0x3E\\\\0x3F
CHR4	= 40-4F:\\\\0x40\\\\0x41\\\\0x42\\\\0x43\\\\0x44\\\\0x45\\\\0x46\\\\0x47\\\\0x48\\\\0x49\\\\0x4A\\\\0x4B\\\\0x4C\\\\0x4D\\\\0x4E\\\\0x4F
CHR5	= 50-5F:\\\\0x50\\\\0x51\\\\0x52\\\\0x53\\\\0x54\\\\0x55\\\\0x56\\\\0x57\\\\0x58\\\\0x59\\\\0x5A\\\\0x5B\\\\0x5C\\\\0x5D\\\\0x5E\\\\0x5F
CHR6	= 60-6F:\\\\0x60\\\\0x61\\\\0x62\\\\0x63\\\\0x64\\\\0x65\\\\0x66\\\\0x67\\\\0x68\\\\0x69\\\\0x6A\\\\0x6B\\\\0x6C\\\\0x6D\\\\0x6E\\\\0x6F
CHR7	= 70-7F:\\\\0x70\\\\0x71\\\\0x72\\\\0x73\\\\0x74\\\\0x75\\\\0x76\\\\0x77\\\\0x78\\\\0x79\\\\0x7A\\\\0x7B\\\\0x7C\\\\0x7D\\\\0x7E\\\\0x7F
CHR8	= 80-8F:\\\\0x80\\\\0x81\\\\0x82\\\\0x83\\\\0x84\\\\0x85\\\\0x86\\\\0x87\\\\0x88\\\\0x89\\\\0x8A\\\\0x8B\\\\0x8C\\\\0x8D\\\\0x8E\\\\0x8F
CHR9	= 90-9F:\\\\0x90\\\\0x91\\\\0x92\\\\0x93\\\\0x94\\\\0x95\\\\0x96\\\\0x97\\\\0x98\\\\0x99\\\\0x9A\\\\0x9B\\\\0x9C\\\\0x9D\\\\0x9E\\\\0x9F
CHRA	= A0-AF:\\\\0xA0\\\\0xA1\\\\0xA2\\\\0xA3\\\\0xA4\\\\0xA5\\\\0xA6\\\\0xA7\\\\0xA8\\\\0xA9\\\\0xAA\\\\0xAB\\\\0xAC\\\\0xAD\\\\0xAE\\\\0xAF
CHRB	= B0-BF:\\\\0xB0\\\\0xB1\\\\0xB2\\\\0xB3\\\\0xB4\\\\0xB5\\\\0xB6\\\\0xB7\\\\0xB8\\\\0xB9\\\\0xBA\\\\0xBB\\\\0xBC\\\\0xBD\\\\0xBE\\\\0xBF
CHRC	= C0-CF:\\\\0xC0\\\\0xC1\\\\0xC2\\\\0xC3\\\\0xC4\\\\0xC5\\\\0xC6\\\\0xC7\\\\0xC8\\\\0xC9\\\\0xCA\\\\0xCB\\\\0xCC\\\\0xCD\\\\0xCE\\\\0xCF
CHRD	= D0-DF:\\\\0xD0\\\\0xD1\\\\0xD2\\\\0xD3\\\\0xD4\\\\0xD5\\\\0xD6\\\\0xD7\\\\0xD8\\\\0xD9\\\\0xDA\\\\0xDB\\\\0xDC\\\\0xDD\\\\0xDE\\\\0xDF
CHRE	= E0-EF:\\\\0xE0\\\\0xE1\\\\0xE2\\\\0xE3\\\\0xE4\\\\0xE5\\\\0xE6\\\\0xE7\\\\0xE8\\\\0xE9\\\\0xEA\\\\0xEB\\\\0xEC\\\\0xED\\\\0xEE\\\\0xEF
CHRF	= F0-FF:\\\\0xF0\\\\0xF1\\\\0xF2\\\\0xF3\\\\0xF4\\\\0xF5\\\\0xF6\\\\0xF7\\\\0xF8\\\\0xF9\\\\0xFA\\\\0xFB\\\\0xFC\\\\0xFD\\\\0xFE\\\\0xFF

# Default rule: create all the resource files, including special case
all:	$(OBJS) norver.r


US.rez:	rezskel
	modskel $* verUS 2,0,0,release,0 '$(PROD)' '$(CHR0)' '$(CHR1)'

Russia.rez:	rezskel
	modskel $* verRussia 0x99,9,9,alpha,0x99 '$(PROD)' '$(CHR2)' '$(CHR3)'

Yugo.rez:	rezskel
	modskel $* verYugoCroatian 0,0,0,final,0 getvers '$(CHR4)' '$(CHR5)'

Ireland.rez:	rezskel
	modskel $* verIreland 3,2,1,release,0 getvers '$(CHR6)' '$(CHR7)'

Thai.rez:	rezskel
	modskel $* verThailand 1,2,3,development,4 getvers '$(CHR8)' '$(CHR9)'

# The following use hex number to generate invalid version numbers

Arabia.rez:	rezskel
	modskel $* verArabia  0xAA,0xA,0xA,alpha,0xAA '$(PROD)' '$(CHRA)' '$(CHRB)'

Britain.rez:	rezskel
	modskel $* verBritain 0xBB,0xB,0xB,beta,0xBB '$(PROD)' '$(CHRC)' '$(CHRD)'

Cyprus.rez:	rezskel
	modskel $* verCyprus  0xCC,0xC,0xC,final,0xCC '$(PROD)' '$(CHRE)' '$(CHRF)'

Denmark.rez:	rezskel
	modskel $* verDenmark 0xDD,0xD,0xD,development,0xDD '$(PROD)' '$(DESC)' '$(COMMENT)'

Latvia.rez:	rezskel
	modskel $* verLatvia  0xEE,0xE,0xE,release,0xEE '$(PROD)' '$(DESC)' '$(COMMENT)'

France.rez:	rezskel
	modskel $* verFrance  0xFF,0xF,0xF,beta,0xFF '$(PROD)' '$(DESC)' '$(COMMENT)'

# Generate at least one decent-sized rVersion resource
Portugal.rez: rezskel
	modskel $* ver$* $(LVERSION) '$(LPROD)' '$(LDESC)' '$(LCOMMENT)'

# Generate a couple of examples with out-of-range country numbers
Bad26.rez:	rezskel
	modskel $* 26 $(VERSION) '$(PROD)' '$(DESC)' '$(COMMENT)'

Bad55.rez:	rezskel
	modskel $* 55 $(VERSION) '$(PROD)' '$(DESC)' '$(COMMENT)'

Bad80.rez:	rezskel
	modskel $* 80 $(VERSION) '$(PROD)' '$(DESC)' '$(COMMENT)'

clean:
	-$(RM) -f $(OBJS) norver.r
	-$(RM) -f $(OBJS:s/.r/.rez/:f)
