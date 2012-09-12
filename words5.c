
/*
 *
 *		HenceFORTH in C		Version 1.2
 *
 *	Written By:
 *		The Missing Link
 *		975 East Ave. Suite 112
 *		Chico, CA 95926
 *		(916) 343-8129
 *
 *		  HenceFORTH in C
 *	   Copyright(c) 1990  MISSING LINK
 *		All Rights Reserved
 *
 *	See README file for redistribution information.
 *
 */
#include "words5.h"
extern header forget;
_back() { 
	HERE SUBTRACT COMMA }
header back = {
	0x80 | 4    ,
	"BACK" ,
	0x80 ,
	&forget ,	  
	_back		};
_begin() { 
	QCOMP HERE ONE }
header begin = {
	0x80 | 5   | 0x40 ,
	"BEGIN" ,
	0x80 ,
	&back ,	  
	_begin		};
_endif() { 
	QCOMP  TWO  QPAIRS  HERE OVER SUBTRACT SWAP STORE }
header endif = {
	0x80 | 5   | 0x40 ,
	"ENDIF" ,
	0x80 ,
	&begin ,	  
	_endif		};
_then() { 
	_endif(); 
}
header then = {
	0x80 | 4   | 0x40 ,
	"THEN" ,
	0x80 ,
	&endif ,	  
	_then		};
_doo() { 
	extern header pdo;
	COMPILE(pdo)  HERE THREE
}
header doo = {
	0x80 | 2   | 0x40 ,
	"DO" ,
	0x80 ,
	&then ,	  
	_doo		};
_loop() { 
	extern header ploop;
	THREE QPAIRS  COMPILE(ploop)  BACK
}
header loop = {
	0x80 | 4   | 0x40 ,
	"LOOP" ,
	0x80 ,
	&doo ,	  
	_loop		};
_plusloop() { 
	extern header pplusloop;
	THREE QPAIRS  COMPILE(pplusloop)  BACK
}
header plusloop = {
	0x80 | 5   | 0x40 ,
	"+LOOP" ,
	0x80 ,
	&loop ,	  
	_plusloop		};
_until() { 
	extern header zerobranch;
	ONE QPAIRS  COMPILE(zerobranch)  BACK
}
header until = {
	0x80 | 5   | 0x40 ,
	"UNTIL" ,
	0x80 ,
	&plusloop ,	  
	_until		};
_endd() { 
	_until(); 
}
header endd = {
	0x80 | 3   | 0x40 ,
	"END" ,
	0x80 ,
	&until ,	  
	_endd		};
_again() { 
	extern header branch;
	ONE QPAIRS  COMPILE(branch) BACK
}
header again = {
	0x80 | 5   | 0x40 ,
	"AGAIN" ,
	0x80 ,
	&endd ,	  
	_again		};
_repeat() { 
	TOR TOR  /*[COMPILE]*/ _again();  
	RFROM RFROM  TWO SUBTRACT  /*[COMPILE]*/ _endif(); 
}
header repeat = {
	0x80 | 6   | 0x40 ,
	"REPEAT" ,
	0x80 ,
	&again ,	  
	_repeat		};
_iff() { 
	extern header zerobranch;
	COMPILE(zerobranch) HERE ZERO COMMA TWO
}
header iff = {
	0x80 | 2   | 0x40 ,
	"IF" ,
	0x80 ,
	&repeat ,	  
	_iff		};
_elsee() { 
	extern header branch;
	TWO QPAIRS  COMPILE(branch)  HERE ZERO COMMA
	    SWAP  TWO /*[COMPILE]*/ _endif();  
	TWO }
header elsee = {
	0x80 | 4   | 0x40 ,
	"ELSE" ,
	0x80 ,
	&iff ,	  
	_elsee		};
_whilee() { /*[COMPILE]*/
	_iff();  
	TWOPLUS }
header whilee = {
	0x80 | 5   | 0x40 ,
	"WHILE" ,
	0x80 ,
	&elsee ,	  
	_whilee		};
_spaces() { 
	ZERO MAX  DASHDUP IF  ZERO DO  SPACE LOOP  ENDIF }
header spaces = {
	0x80 | 6    ,
	"SPACES" ,
	0x80 ,
	&whilee ,	  
	_spaces		};
_begindigits() { 
	PAD HLD STORE }
header begindigits = {
	0x80 | 2    ,
	"<#" ,
	0x80 ,
	&spaces ,	  
	_begindigits		};
_enddigits() { 
	DROP  DROP  HLD  FETCH  PAD  OVER SUBTRACT }
header enddigits = {
	0x80 | 2    ,
	"#>" ,
	0x80 ,
	&begindigits ,	  
	_enddigits		};
_sign() { 
	ROT  ZEROLESS IF  LIT(0X2D) HOLD  ENDIF }
header sign = {
	0x80 | 4    ,
	"SIGN" ,
	0x80 ,
	&enddigits ,	  
	_sign		};
_adigit() { 
	BASE FETCH  MSMOD  ROT LIT(9) OVER  LESS IF
	    LIT(7) PLUS  ENDIF  LIT(0X30) PLUS HOLD }
header adigit = {
	0x80 | 1    ,
	"#" ,
	0x80 ,
	&sign ,	  
	_adigit		};
_digits() { 
	BEGIN  ADIGIT OVER OVER  OR  ZEROEQUALS UNTIL }
header digits = {
	0x80 | 2    ,
	"#S" ,
	0x80 ,
	&adigit ,	  
	_digits		};
_ddotr() { 
	TOR  SWAP  OVER  DABS  BEGINDIGITS  DIGITS  SIGN
	    ENDDIGITS  RFROM  OVER  SUBTRACT  SPACES  TYPE }
header ddotr = {
	0x80 | 3    ,
	"D.R" ,
	0x80 ,
	&digits ,	  
	_ddotr		};
_ddot() { 
	ZERO  DDOTR SPACE }
header ddot = {
	0x80 | 2    ,
	"D." ,
	0x80 ,
	&ddotr ,	  
	_ddot		};
_dotr() { 
	TOR  STOD  RFROM  DDOTR }
header dotr = {
	0x80 | 2    ,
	".R" ,
	0x80 ,
	&ddot ,	  
	_dotr		};
_dot() { 
	STOD  DDOT }
header dot = {
	0x80 | 1    ,
	"." ,
	0x80 ,
	&dotr ,	  
	_dot		};
_question() { 
	FETCH DOT }
header question = {
	0x80 | 1    ,
	"?" ,
	0x80 ,
	&dot ,	  
	_question		};
_list() { 
	DECIMAL  CR DUP SCR STORE
	    DOTQUOTE("SCR # ")  DOT  LIT(0X10) ZERO DO  CR I THREE DOTR SPACE
	    I SCR FETCH DOTLINE  LOOP  CR }
header list = {
	0x80 | 4    ,
	"LIST" ,
	0x80 ,
	&question ,	  
	_list		};
_indexx() { 
	LIT(0X0C) EMIT /* FORM FEED */  CR ONEPLUS SWAP
	    DO  CR I THREE DOTR SPACE
	    ZERO I DOTLINE
	    QTERMINAL IF  LEAVE  ENDIF  LOOP }
header indexx = {
	0x80 | 5    ,
	"INDEX" ,
	0x80 ,
	&list ,	  
	_indexx		};
_triad() { 
	LIT(0X0C) EMIT /* FF */  THREE SLASH  THREE STAR  THREE OVER
	    PLUS SWAP DO  CR I LIST  LOOP  CR  LIT(0X0F) MESSAGE  CR }
header triad = {
	0x80 | 5    ,
	"TRIAD" ,
	0x80 ,
	&indexx ,	  
	_triad		};
_vlist() { 
	LIT(0X80) OUT STORE  CONTEXT FETCH FETCH
	    BEGIN  OUT FETCH  CSLASHL  GREATER IF  CR ZERO OUT STORE  ENDIF
	    DUP IDDOT  SPACE  SPACE  PFA LFA FETCH
	    DUP  ZEROEQUALS  QTERMINAL  OR  UNTIL  DROP }
header vlist = {
	0x80 | 5    ,
	"VLIST" ,
	0x80 ,
	&triad ,	  
	_vlist		};
header dwidth = {
	0x80 | 6    ,
	"DWIDTH" ,
	0x80 ,
	&vlist ,	  
	docon ,
	(cell) 16    	
};
_dotasc() { 
	CFETCH DUP LIT(32) LESS OVER  LIT(126)  GREATER 
	    OR IF  DROP  DOTQUOTE(".")  ELSE  EMIT  ENDIF }
header dotasc = {
	0x80 | 4    ,
	".ASC" ,
	0x80 ,
	&dwidth ,	  
	_dotasc		};
_dotstr() { 
	ZERO DO  DUP I PLUS DOTASC  LOOP  DROP }
header dotstr = {
	0x80 | 4    ,
	".STR" ,
	0x80 ,
	&dotasc ,	  
	_dotstr		};
_dotnum() { 
	CFETCH ZERO  BEGINDIGITS  ADIGIT ADIGIT  ENDDIGITS  TYPE }
header dotnum = {
	0x80 | 4    ,
	".NUM" ,
	0x80 ,
	&dotstr ,	  
	_dotnum		};
_dothex() { 
	ZERO DO  DUP I PLUS DOTNUM  SPACE  LOOP  DROP }
header dothex = {
	0x80 | 4    ,
	".HEX" ,
	0x80 ,
	&dotnum ,	  
	_dothex		};
_dotadr() { 
	ZERO  BEGINDIGITS  LIT(58) HOLD  LIT(8)  ZERO  DO
	    ADIGIT  LOOP  ENDDIGITS  TYPE }
header dotadr = {
	0x80 | 4    ,
	".ADR" ,
	0x80 ,
	&dothex ,	  
	_dotadr		};
_dump() { 
	BASE FETCH  TOR  HEX  ONEPLUS  OVER  PLUS  SWAP  DO CR  I DOTADR
	    I DWIDTH DOTHEX  SPACE  I DWIDTH DOTSTR  
	    QTERMINAL IF  LEAVE  ENDIF  DWIDTH  PLUSLOOP
	    RFROM  BASE STORE CR }
header dump = {
	0x80 | 4    ,
	"DUMP" ,
	0x80 ,
	&dotadr ,	  
	_dump		};
_t() { 
	CR   DUP  SCR FETCH PLINE TYPE  DUP DOT }
header t = {
	0x80 | 1    ,
	"T" ,
	0x80 ,
	&dump ,	  
	_t		};
_p() { 
	TOR HERE  CSLASHL  ONEPLUS BLANKS   ONE WORD
	    HERE ONEPLUS R SCR FETCH PLINE CMOVE UPDATE  RFROM }
header p = {
	0x80 | 1    ,
	"P" ,
	0x80 ,
	&t ,	  
	_p		};
_u() { 
	ONEPLUS  DUP LIT(15) DO  I ONESUBTRACT  SCR FETCH PLINE  DROP
	    I SCR FETCH PLINE CMOVE  UPDATE  LIT(-1)  PLUSLOOP P }
header u = {
	0x80 | 1    ,
	"U" ,
	0x80 ,
	&p ,	  
	_u		};
_l() { 
	SCR FETCH LIST }
header l = {
	0x80 | 1    ,
	"L" ,
	0x80 ,
	&u ,	  
	_l		};
_n() { 
	SCR FETCH ONEPLUS LIST }
header n = {
	0x80 | 1    ,
	"N" ,
	0x80 ,
	&l ,	  
	_n		};
_b() { 
	SCR FETCH ONESUBTRACT LIST }
header b = {
	0x80 | 1    ,
	"B" ,
	0x80 ,
	&n ,	  
	_b		};
_x() { 
	DUP ONEPLUS LIT(16) SWAP DO  I SCR FETCH PLINE DROP
	    I ONESUBTRACT  SCR FETCH PLINE CMOVE  UPDATE  LOOP
	    LIT(15) SCR FETCH PLINE BLANKS  UPDATE }
header x = {
	0x80 | 1    ,
	"X" ,
	0x80 ,
	&b ,	  
	_x		};
_wipe() { 
	LIT(16) ZERO DO  I SCR FETCH PLINE BLANKS  UPDATE  LOOP }
header wipe = {
	0x80 | 4    ,
	"WIPE" ,
	0x80 ,
	&x ,	  
	_wipe		};
_flush() { 
	NUMBERBUF ZERO DO  LIT(UFLAG-1) BUFFER DROP  LOOP }
header flush = {
	0x80 | 5    ,
	"FLUSH" ,
	0x80 ,
	&wipe ,	  
	_flush		};
_thru() { 
	ONEPLUS SWAP DO  I ZERO DDOT  I LOAD  QTERMINAL
	    LIT(9) QERROR  LOOP }
header thru = {
	0x80 | 4    ,
	"THRU" ,
	0x80 ,
	&flush ,	  
	_thru		};
_bye() { 
	goodbye(); 
}
header bye = {
	0x80 | 3    ,
	"BYE" ,
	0x80 ,
	&thru ,	  
	_bye		};

