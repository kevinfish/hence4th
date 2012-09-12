CFLAGS = -O
MFLAGS = -s
MACROS = prims1.m4 prims2.m4 words1.m4 words2.m4 words3.m4 words4.m4 words5.m4
CPROGS = misc.c
INCLUDES = compiler.h declare.h	misc.h struct.h words.h config.h Makefile $(MACROS:.m4=.h)

Hence4th: $(MACROS:.m4=.o) $(CPROGS:.c=.o)
	gcc $(MACROS:.m4=.o) $(CPROGS:.c=.o) -o Hence4th

$(MACROS:.m4=.o) $(CPROGS:.c=.o): $(INCLUDES)

# $(MACROS:.m4=.c): Makefile m4define

.m4.c:
	cat m4define $<	| m4 $(MFALGS)| cb | tr -s "\012" "\012" > $*.c
.m4.o:
	make $*.c


lint:
	lint -p -Dlint $(MACROS:.m4=.c) misc.c

dictionary:
	cat m4declare *.m4 | m4	| cb | tr -s "\012" "\012" > words.h

.SUFFIXES: .o .m4
