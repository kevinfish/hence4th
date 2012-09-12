/*
 *
 *		HenceFORTH in C		Version 1.2
 *
 *		The Missing Link
 *		975 East Ave. Suite 112
 *		Chico, CA 95926
 *		(916) 343-8129
 *
 *		  HenceFORTH in C
 *	   (c) Copyright 1990 MISSING LINK 
 *		All Rights Reserved
 */

#include "misc.h"

#include "config.h"

#if UNIX
# include <sys/param.h>
# include <termio.h>
# include <fcntl.h>
# include <sys/ioctl.h>
#endif

#if MIX
# include <io.h>
# include <fcntl.h>
# include <dos.h>
#endif

#include <setjmp.h>

#include "declare.h"
#include "struct.h"

#include "compiler.h"
#include "words.h"
 
a_cell datastack[DSMAX+3];
a_cell *dsp = datastack + DSMAX;	/* data stack pointer */
a_cell retstack[RSMAX];
a_cell *rsp = retstack + RSMAX;	/* return stack pointer */
jmp_buf jmpstack[RSMAX];
jmp_buf *jsp = jmpstack + RSMAX;	/* a stack of jump buffers
						tied to the return stack */

extern header cold, warm, TOPWORD, forth;
 
extern char memory[];
 
cell origin[17] = {
	JMP,
	(cell) &cold.pf,	/* word aligned vector to cold */
	JMP,
	(cell) &warm.pf,	/* word aligned vector to warm */
	0,	1,		/* cpu, and revision parameters */
	(cell) &TOPWORD,	/* topmost word in forth vocabulary */
	8,			/* backspace char */
	FIRSTBUF-32*sizeof(a_cell),	/* initial user area */
#if MIX
	(cell) datastack+DSMAX,	/* initial top of stack */
	(cell) retstack+RSMAX,	/* initial top of return stack */
#else
	(cell) (datastack+DSMAX), /* initial top of stack */
	(cell) (retstack+RSMAX),  /* initial top of return stack */
#endif
	(cell) memory,		/* terminal input buffer */
	HEADMAX,		/* initial name field width */
	1,			/* initial warning */
#if MIX
	(cell) memory+TIBSIZE,	/* initial fence */
	(cell) memory+TIBSIZE,	/* cold start value for dp */
#else
	(cell) (memory+TIBSIZE), /* initial fence */
	(cell) (memory+TIBSIZE), /* cold start value for dp */
#endif
	(cell) &forth.pf4	/* cold start value for voc-link */
}
	;	/* boot up paramaters */
 
char memory[EOM];	/* the user dictionary/memory */
 
	/* virtual machine registers: */
 
char *up, *ip;	/* user variable pointer, interpretive pointer */
cell *w;		/* where we came from (last cfa executed) */
 
 
jmp_buf to_cold, to_quit, to_warm, to_abort;
 
char *sysalign(x)	/* alignment function for this machine */
char *x;
{
#if WORD_ALIGNED
	return(x = x + ((int) x & 1));
#else
	return(x);
#endif
}

#if MIX
union REGS inregs, outregs;
#endif
 
int getkey()	/* get a key w/o echo or waiting (0 if none available) */
{

#if UNIX
	char c;

	if(read(0, &c, 1) == 0)	c = 0;
	return(c);
#endif

#if PCC
	return(_os(6,0xffff));
#endif 

#if MIX
	inregs.h.ah=6;
	inregs.h.dl=255;
	inregs.h.al=0;
	return(intdos(&inregs,&outregs)&0xff);
#endif
}	

char *fname[] = { "dr0", "dr1" };	/* file names */
 
int fhandle[2];


#if UNIX

struct termio tty, otty;	/* modify/restore terminal settings */

#endif


void initsys()	/* Initialize */
{
	int tmp;

#if UNIX
	ioctl(0, TCGETA, &tty);	/* get tty settings */
	ioctl(0, TCGETA, &otty); /* another for restoring later */
#ifndef DEBUG
	tty.c_lflag &= ~ICANON; /* no erase/kill input processing */
	tty.c_lflag &= ~ECHO;	/* no echo */
	tty.c_cc[4] = 0;	/* VMIN - don't wait for any characters */
	tty.c_cc[5] = 0;	/* VTIME - or any minimum time */
	ioctl(0, TCSETA, &tty);	/* reset tty settings */

	tmp = fcntl(0, F_GETFL, 0);
	tmp |= O_NDELAY;
	fcntl(0, F_SETFL, tmp);
#endif
#endif
	for(tmp = 0; tmp <= 1; tmp++) {
		if((fhandle[tmp] =
			open(fname[tmp],
#if UNIX
				O_RDWR|O_CREAT, 0644)) == -1)	{
#endif
#if MIX
				O_RDWR|O_CREAT)) == -1)	{
#endif
#if PCC
				(char) 2)) == -1)	{
#endif
				DOTQUOTE("Hence4th: Can't open a screen file");
		}
	}
}
 
 
/* next: execute itc, forths inner interpreter */
void next() {
loop:
	w = (cell *)*((cell *)ip)++;
#if PCC
	(**w)();
#else
	(*(void(*)())*w)();
#endif
	goto loop;
}
 
 
/* nest into Indirect Threaded code word */
void docolon() {
	rpush(ip);
	ip = (char *) (w + 1);
	/* only hit next if not returning from invocation*/
	if(!setjmp(*--jsp))	next();
}
 
void docon() { push(*(w + 1)); }
 
void dovar() { push(w + 1); }
 
void douser() { push(up + *(w + 1)); }
 
void dodoes() {
	rpush(ip);
	ip = (char *) *(w + 1);
	push(w + 2);
	if(!setjmp(*--jsp))	next();
}
 
void dovoc() { LIT(sizeof(a_cell)) PLUS CONTEXT STORE  SEMIS }
 
void (*dummy1)() = dovoc;	/* fake itc for vocabularies */
 
cell *dummy2 = (cell *) &dummy1;


void dointerpret() {  BEGIN DASHFIND  IF  STATE FETCH LESS
	IF CFA COMMA  ELSE   CFA  EXECUTE  ENDIF
	QSTACK  ELSE  HERE  NUMBER  DPL  FETCH  ONEPLUS
	IF /*[COMPILE]*/ DLITERAL  ELSE  DROP  /*[COMPILE]*/ LITERAL
	ENDIF   QSTACK  ENDIF  AGAIN
}

#if MIX
 cell *pinterpret = dointerpret;
#else
 cell *pinterpret = (cell *) dointerpret;
#endif

cell *dummyi = (cell *)&pinterpret;

void doquit() {  setjmp(to_quit); ZERO BLK STORE  /*[COMPILE]*/ LEFTBRACKET
	BEGIN  RPSTORE  CR  QUERY  rpush(0); if(!setjmp(*--jsp)) dointerpret();
	STATE FETCH ZEROEQUALS IF DOTQUOTE("  OK")  ENDIF   AGAIN
}


void doabort() {
	setjmp(to_abort); SPSTORE  DECIMAL   DRZERO
	CR  DOTQUOTE("HenceFORTH in C  Ver 1.2")
	CR  DOTQUOTE("(c) Copyright 1990  Kevin Haddock")
	CR  DOTQUOTE("All Rights Reserved")
	/*[COMPILE]*/  FORTH  DEFINITIONS  doquit();
}
 
void start(x)
cell x;
{
	up = (char *) origin[8];
	do { *((cell *) up + x) = *(origin + 6 + x); }  while( x-- > 0);
}
 
void dowarm() {
	setjmp(to_warm);
	start(7);	/* index to warning */
	doabort();	/* this out of necessity must be different */
}
 
void docold() {
	setjmp(to_cold);
	initsys();
	forth.pf3 = *(origin + 6);	/* highest word */
	start(10);
	dowarm();
}
 
void main(){ docold(); }

void goodbye() {
#if UNIX
	int tmp;

	ioctl(0, TCSETA, &otty);

	tmp = fcntl(0, F_GETFL,0);
	tmp &= ~O_NDELAY;
	fcntl(0, F_SETFL, tmp);
#endif
	
	close(fhandle[0]);
	close(fhandle[1]);
	exit();
}

