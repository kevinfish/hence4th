 
/*
 *	HenceFORTH in C   Ver 1.2
 *	(c) Copyright 1990 MISSING LINK
 *	All Rights Reserved
 *
 *	File:	compiler.h
 *	Desc:	Forth psudocode compiler and stack macros.
 */
 
 
#define pop		(*dsp++)
#define push(x)		(*--dsp = (cell)(x))
#define LIT(x)		(push(x));
#define ZERO		LIT(0)
#define ONE		LIT(1)
#define TWO		LIT(2)
#define rpop		(*rsp++)
#define rpush(x)	(*--rsp = (cell)(x))
#define TOR		(rpush(pop));
#define RFROM		(push(rpop));
#define I		(push(*rsp));
#define R		I
 
#define IF		if(pop) {
#define ELSE		} else {
#define ENDIF		}
#define THEN		ENDIF
#define DO		rpush(*(dsp + 1)); rpush(pop); *dsp++; do {
#define LOOP		} while(++(*rsp) < *(rsp + 1)); rsp += 2;
#define PLUSLOOP	} while((*dsp >= 0) ? \
				(*rsp += pop) < *(rsp + 1) : \
				(*rsp += pop) > *(rsp + 1)); \
				rsp += 2;
#define LEAVE		(*(rsp + 1) = *rsp);
#define BEGIN		do {
#define AGAIN		} while(1);
#define UNTIL		} while(!pop);
#define	WHILE		if(!pop) break;
#define REPEAT		AGAIN
#define END		UNTIL
#define COMPILE(x)	LIT(&x.cf) COMMA
#define SEMICODE(x)	LIT(x)  LATEST PFA CFA STORE
#define DOTQUOTE(x)	{ char *i = x; do putchar(*i++); while (*i); }
#define DOES(x)		SEMICODE(dodoes) LIT(x)  LATEST PFA STORE

