
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
#include "words4.h"
extern header msmod;
header use = {
	0x80 | 3    ,
	"USE" ,
	0x80 ,
	&msmod ,	  
	dovar ,
#if MIX
	(cell) memory+FIRSTBUF    	
#else
	(cell) (memory+FIRSTBUF)
#endif
};
header prev = {
	0x80 | 4    ,
	"PREV" ,
	0x80 ,
	&use ,	  
	dovar ,
#if MIX
	(cell) memory+FIRSTBUF    	
#else
	(cell) (memory+FIRSTBUF)
#endif
};
_plusbuf() { 
	LIT(BPERBUF + 2 * sizeof(a_cell))  PLUS  DUP
	    LIMIT EQUALS IF  DROP FIRST  ENDIF  DUP PREV FETCH SUBTRACT }
header plusbuf = {
	0x80 | 4    ,
	"+BUF" ,
	0x80 ,
	&prev ,	  
	_plusbuf		};
_update() { 
	PREV  FETCH FETCH  LIT(UFLAG) OR  PREV FETCH STORE }
header update = {
	0x80 | 6    ,
	"UPDATE" ,
	0x80 ,
	&plusbuf ,	  
	_update		};
_emptybuffers() { 
	FIRST  LIMIT  OVER  SUBTRACT  ERASEE }
header emptybuffers = {
	0x80 | 13    ,
	"EMPTY-BUFFERS" ,
	0x80 ,
	&update ,	  
	_emptybuffers		};
_drzero() { 
	ZERO OFFSET STORE }
header drzero = {
	0x80 | 3    ,
	"DR0" ,
	0x80 ,
	&emptybuffers ,	  
	_drzero		};
_drone() { 
	LIT(BPERDRIVE) OFFSET STORE }
header drone = {
	0x80 | 3    ,
	"DR1" ,
	0x80 ,
	&drzero ,	  
	_drone		};
_buffer() { 
	USE FETCH DUP TOR  /* BUFFER ADDR TO BE ASSIGNED */
	BEGIN  PLUSBUF  UNTIL  /* AVOID PREV */  USE STORE /* FOR NEXT TIME */
	R FETCH ZEROLESS /* TEST FOR UPDATE IN THIS BUFFER */
	IF /* UPDATED, FLUSH TO DISK */
	R LIT(sizeof(a_cell)) PLUS  /* STORAGE LOCATION */
	R FETCH  LIT(~UFLAG) AND  /* ITS BLOCK */
	ZERO  RSLASHW  /* WRITE SECTOR TO DISK */
	ENDIF
	    R STORE  /* WRITE NEW BLOCK # INTO THIS BUFFER */
	R PREV STORE  /* ASSIGN THIS BUFFER AS PREV */
	RFROM LIT(sizeof(a_cell)) PLUS }
header buffer = {
	0x80 | 6    ,
	"BUFFER" ,
	0x80 ,
	&drone ,	  
	_buffer		};
_block() { 
	OFFSET FETCH PLUS TOR  /* RETAIN BLOCK# ON RETURN STACK */
	PREV FETCH DUP FETCH R SUBTRACT DUP PLUS  /* BLOCK = PREV */
	IF /* NOT PREV */
	BEGIN  PLUSBUF  ZEROEQUALS  /* TRUE UPON REACHING PREV */
	IF /* WRAPPED */  DROP  R BUFFER
	    DUP R ONE  RSLASHW  /* READ SECTOR FROM DISK */
	LIT(sizeof(a_cell)) SUBTRACT /* BACKUP */
	ENDIF
	    DUP FETCH R SUBTRACT  DUP PLUS ZEROEQUALS
	    UNTIL  /* WITH BUFFER ADDRESS */
	DUP PREV STORE
	    ENDIF
	    RFROM DROP  LIT(sizeof(a_cell)) PLUS }
header block = {
	0x80 | 5    ,
	"BLOCK" ,
	0x80 ,
	&buffer ,	  
	_block		};
_pline() { 
	TOR  CSLASHL  BSLASHBUF  SSMOD  RFROM  BSLASHSCR
	    STAR PLUS  BLOCK PLUS CSLASHL }
header pline = {
	0x80 | 6    ,
	"\050LINE\051" ,
	0x80 ,
	&block ,	  
	_pline		};
_dotline() { 
	PLINE  DASHTRAILING  TYPE }
header dotline = {
	0x80 | 5    ,
	".LINE" ,
	0x80 ,
	&pline ,	  
	_dotline		};
_message() { 
	WARNING FETCH IF /* disk is available */ DASHDUP IF
	    LIT(4) OFFSET FETCH  BSLASHSCR  SLASH  SUBTRACT DOTLINE ENDIF
	    ELSE  DOTQUOTE("MSG # ")    DOT  ENDIF }
header message = {
	0x80 | 7    ,
	"MESSAGE" ,
	0x80 ,
	&dotline ,	  
	_message		};
_load() { 
	BLK FETCH TOR  IN FETCH TOR  ZERO IN STORE  BSLASHSCR STAR
	    BLK STORE   rpush(0);   
	if(!setjmp(*--jsp))  dointerpret();
	RFROM IN STORE   RFROM BLK STORE }
header load = {
	0x80 | 4    ,
	"LOAD" ,
	0x80 ,
	&message ,	  
	_load		};
_nextscreen() { 
	QLOADING  ZERO IN STORE  BSLASHSCR BLK FETCH  OVER
	    MOD SUBTRACT  BLK  PLUSSTORE }
header nextscreen = {
	0x80 | 3   | 0x40 ,
	"-->" ,
	0x80 ,
	&load ,	  
	_nextscreen		};
/* system independent */
_rslashw() { 
	register cell flag = pop, nbytes;
	register cell block = pop;
	register char *addr = (char *)pop;
	register cell file = block < BPERDRIVE ?  0 : 1; /* first BPERDRIVE goes
								to "dr0", the rest
								to "dr1" */
	if(lseek(fhandle[file], (long) ((block * BPERSCR)
	    - (BPERDRIVE * file)) * BPERBUF, 0) == -1)	{
		LIT(8) ERROR
	}
	if(flag)	{
		nbytes = read(fhandle[file], addr, BPERBUF);
		if(nbytes != 0 && nbytes != BPERBUF)	{
			LIT(8) ERROR
		}
	}  
	else		{
		if(write(fhandle[file], addr, BPERBUF) != BPERBUF)	{
			LIT(8) ERROR
		}
	}
}
header rslashw = {
	0x80 | 3    ,
	"R/W" ,
	0x80 ,
	&nextscreen ,	  
	_rslashw		};
_tick() { 
	DASHFIND  ZEROEQUALS  ZERO  QERROR  DROP
	    /* [COMPILE] */ LITERAL }
header tick = {
	0x80 | 1   | 0x40 ,
	"\047" ,
	0x80 ,
	&rslashw ,	  
	_tick		};
_forget() {  /*[COMPILE]*/
	TICK  NFA
	    DUP  FENCE FETCH SUBTRACT ZEROLESS LIT(0X15) QERROR
	    TOR VOCLINK FETCH /* START WITH LATEST VOCABULARY */
	BEGIN   R OVER SUBTRACT ZEROLESS  WHILE
	    /*[COMPILE]*/ FORTH DEFINITIONS FETCH  DUP VOCLINK STORE REPEAT
	    BEGIN  DUP LFA BEGIN  PFA  LFA  FETCH
	    DUP  R  SUBTRACT ZEROLESS  UNTIL   OVER  CFA  STORE
	    FETCH DASHDUP ZEROEQUALS UNTIL   RFROM  DP STORE  }
header forget = {
	0x80 | 6    ,
	"FORGET" ,
	0x80 ,
	&tick ,	  
	_forget		};

