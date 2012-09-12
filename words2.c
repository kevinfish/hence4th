
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
#include "words2.h"
extern header dashdup;
_traverse() { 
	SWAP BEGIN OVER PLUS LIT(0x7F) OVER CFETCH LESS UNTIL
	    SWAP DROP }
header traverse = {
	0x80 | 8    ,
	"TRAVERSE" ,
	0x80 ,
	&dashdup ,	  
	_traverse		};
_latest() { 
	CURRENT FETCH FETCH }
header latest = {
	0x80 | 6    ,
	"LATEST" ,
	0x80 ,
	&traverse ,	  
	_latest		};
_lfa() { 
	LIT( 2 * sizeof(a_cell)) SUBTRACT }
header lfa = {
	0x80 | 3    ,
	"LFA" ,
	0x80 ,
	&latest ,	  
	_lfa		};
_cfa() { 
	LIT(sizeof(a_cell)) SUBTRACT }
header cfa = {
	0x80 | 3    ,
	"CFA" ,
	0x80 ,
	&lfa ,	  
	_cfa		};
_nfa() { 
	LIT(2 * sizeof(a_cell) + 1) SUBTRACT LIT(-1) TRAVERSE }
header nfa = {
	0x80 | 3    ,
	"NFA" ,
	0x80 ,
	&cfa ,	  
	_nfa		};
_pfa() { 
	ONE TRAVERSE LIT(2 * sizeof(a_cell) + 1) PLUS  ALIGN }
header pfa = {
	0x80 | 3    ,
	"PFA" ,
	0x80 ,
	&nfa ,	  
	_pfa		};
_storecsp() { 
	SPFETCH CSP STORE }
header storecsp = {
	0x80 | 4    ,
	"!CSP" ,
	0x80 ,
	&pfa ,	  
	_storecsp		};
_qerror() { 
	SWAP IF  ERROR  ELSE  DROP  ENDIF }
header qerror = {
	0x80 | 6    ,
	"?ERROR" ,
	0x80 ,
	&storecsp ,	  
	_qerror		};
_qcomp() { 
	STATE FETCH ZEROEQUALS LIT(0x11) QERROR }
header qcomp = {
	0x80 | 5    ,
	"?COMP" ,
	0x80 ,
	&qerror ,	  
	_qcomp		};
_qexec() { 
	STATE FETCH LIT(0x12) QERROR }
header qexec = {
	0x80 | 5    ,
	"?EXEC" ,
	0x80 ,
	&qcomp ,	  
	_qexec		};
_qpairs() { 
	SUBTRACT LIT(0x13) QERROR }
header qpairs = {
	0x80 | 6    ,
	"?PAIRS" ,
	0x80 ,
	&qexec ,	  
	_qpairs		};
_qcsp() { 
	SPFETCH CSP FETCH SUBTRACT LIT(0x14) QERROR }
header qcsp = {
	0x80 | 4    ,
	"?CSP" ,
	0x80 ,
	&qpairs ,	  
	_qcsp		};
_qloading() { 
	BLK FETCH ZEROEQUALS LIT(0x16) QERROR }
header qloading = {
	0x80 | 8    ,
	"?LOADING" ,
	0x80 ,
	&qcsp ,	  
	_qloading		};
_compile() { 
	QCOMP LIT(ip) DUP LIT(sizeof(a_cell)) PLUS
	    ip = (char *) pop; 
	FETCH COMMA }
header compile = {
	0x80 | 7    ,
	"COMPILE" ,
	0x80 ,
	&qloading ,	  
	_compile		};
_leftbracket() { 
	ZERO STATE STORE }
header leftbracket = {
	0x80 | 1   | 0x40 ,
	"[" ,
	0x80 ,
	&compile ,	  
	_leftbracket		};
_rightbracket() { 
	LIT(0xC0) STATE STORE }
header rightbracket = {
	0x80 | 1    ,
	"]" ,
	0x80 ,
	&leftbracket ,	  
	_rightbracket		};
_smudge() { 
	LATEST LIT(0x20) TOGGLE }
header smudge = {
	0x80 | 6    ,
	"SMUDGE" ,
	0x80 ,
	&rightbracket ,	  
	_smudge		};
_hex() { 
	LIT(0x10) BASE STORE }
header hex = {
	0x80 | 3    ,
	"HEX" ,
	0x80 ,
	&smudge ,	  
	_hex		};
_decimal() { 
	LIT(10) BASE STORE }
header decimal = {
	0x80 | 7    ,
	"DECIMAL" ,
	0x80 ,
	&hex ,	  
	_decimal		};
_psemicode() { 
	push(ip); 
	LATEST PFA CFA STORE SEMIS }
header psemicode = {
	0x80 | 7    ,
	"\050;CODE\051" ,
	0x80 ,
	&decimal ,	  
	_psemicode		};
_semicode() { 
	QCSP COMPILE(psemicode) /*[COMPILE]*/ LEFTBRACKET }
header semicode = {
	0x80 | 5    ,
	";CODE" ,
	0x80 ,
	&psemicode ,	  
	_semicode		};
_builds() { 
	ZERO CONSTANT }
header builds = {
	0x80 | 7    ,
	"<BUILDS" ,
	0x80 ,
	&semicode ,	  
	_builds		};
_does() { 
	push(ip); 
	LATEST PFA STORE SEMICODE(dodoes) SEMIS }
header does = {
	0x80 | 5    ,
	"DOES>" ,
	0x80 ,
	&builds ,	  
	_does		};
_count() { 
	DUP ONEPLUS SWAP CFETCH }
header count = {
	0x80 | 5    ,
	"COUNT" ,
	0x80 ,
	&does ,	  
	_count		};
_type() { 
	DASHDUP IF  OVER  PLUS  SWAP  DO I CFETCH EMIT  LOOP
	    ELSE DROP ENDIF }
header type = {
	0x80 | 4    ,
	"TYPE" ,
	0x80 ,
	&count ,	  
	_type		};
_dashtrailing() { 
	DUP ZERO DO  OVER OVER  PLUS ONE SUBTRACT
	    CFETCH BL SUBTRACT IF LEAVE ELSE ONE SUBTRACT ENDIF  LOOP }
header dashtrailing = {
	0x80 | 9    ,
	"-TRAILING" ,
	0x80 ,
	&type ,	  
	_dashtrailing		};
_pdotquote() { 
	LIT(ip) COUNT DUP  ONEPLUS
	    LIT(ip) PLUS  ALIGN  ip = (char *)pop;  
	TYPE }
header pdotquote = {
	0x80 | 4    ,
	"\050.\"\051" ,
	0x80 ,
	&dashtrailing ,	  
	_pdotquote		};
_dotquote() { 
	LIT(0x22) STATE FETCH  IF  COMPILE(pdotquote)
		WORD HERE CFETCH ONEPLUS ALLOT HERE ALIGN DP STORE
	    ELSE  WORD HERE COUNT TYPE ENDIF }
header dotquote = {
	0x80 | 2   | 0x40 ,
	".\"" ,
	0x80 ,
	&pdotquote ,	  
	_dotquote		};
_expect() { 
	OVER PLUS OVER DO KEY DUP LIT(0X0E) PLUSORIGIN FETCH
	    EQUALS IF DROP  LIT(8) OVER I EQUALS  DUP  RFROM  TWO SUBTRACT
	    PLUS  TOR  SUBTRACT ELSE  DUP  LIT(EOL) EQUALS IF  LEAVE  DROP
	    BL  ZERO  ELSE DUP  ENDIF  I CSTORE  ZERO I ONEPLUS CSTORE  ZERO
	    I TWOPLUS CSTORE  ENDIF  EMIT  LOOP  DROP }
header expect = {
	0x80 | 6    ,
	"EXPECT" ,
	0x80 ,
	&dotquote ,	  
	_expect		};
_query() { 
	TIB FETCH LIT(0x50) EXPECT  ZERO IN STORE }
header query = {
	0x80 | 5    ,
	"QUERY" ,
	0x80 ,
	&expect ,	  
	_query		};
_null() { 
	BLK FETCH  IF  ONE BLK PLUSSTORE  ZERO IN STORE
	    BLK FETCH LIT(1024 / BPERBUF - 1) AND ZEROEQUALS IF QEXEC
	    /* RFROM DROP */ SEMIS  ENDIF  ELSE /* RFROM DROP */ SEMIS  ENDIF }
header null = {
	0x80 | 1   | 0x40 ,
	"\000" ,
	0x80 ,
	&query ,	  
	_null		};
_fill() { 
	SWAP TOR  OVER CSTORE  DUP ONEPLUS  RFROM ONE SUBTRACT CMOVE }
header fill = {
	0x80 | 4    ,
	"FILL" ,
	0x80 ,
	&null ,	  
	_fill		};
_erasee() { 
	ZERO FILL }
header erasee = {
	0x80 | 5    ,
	"ERASE" ,
	0x80 ,
	&fill ,	  
	_erasee		};
_blanks() { 
	BL FILL }
header blanks = {
	0x80 | 6    ,
	"BLANKS" ,
	0x80 ,
	&erasee ,	  
	_blanks		};
_hold() { 
	LIT(-1) HLD PLUSSTORE  HLD FETCH CSTORE }
header hold = {
	0x80 | 4    ,
	"HOLD" ,
	0x80 ,
	&blanks ,	  
	_hold		};
_pad() { 
	HERE LIT(0X44) PLUS }
header pad = {
	0x80 | 3    ,
	"PAD" ,
	0x80 ,
	&hold ,	  
	_pad		};
_word() { 
	BLK FETCH IF  BLK FETCH  BLOCK  ELSE  TIB FETCH  ENDIF
	    IN FETCH PLUS SWAP  ENCLOSE  HERE LIT(0X22) BLANKS
	    IN PLUSSTORE  OVER SUBTRACT TOR  R HERE CSTORE
	    PLUS HERE ONEPLUS  RFROM CMOVE }
header word = {
	0x80 | 4    ,
	"WORD" ,
	0x80 ,
	&pad ,	  
	_word		};
_pnumber() { 
	BEGIN ONEPLUS DUP TOR  CFETCH BASE FETCH
	    DIGIT  WHILE  SWAP  BASE FETCH USTAR  DROP ROT
	    BASE FETCH USTAR DPLUS
	    DPL FETCH  ONEPLUS IF  ONE DPL PLUSSTORE
	    ENDIF  RFROM  REPEAT  RFROM }
header pnumber = {
	0x80 | 8    ,
	"\050NUMBER\051" ,
	0x80 ,
	&word ,	  
	_pnumber		};
_number() { 
	ZERO ZERO ROT DUP ONEPLUS CFETCH LIT(0X2D) EQUALS DUP TOR
	    PLUS LIT(-1) BEGIN  DPL STORE PNUMBER  DUP CFETCH BL SUBTRACT
	    WHILE  DUP CFETCH LIT(0X2E) SUBTRACT ZERO QERROR
	    ZERO REPEAT  DROP RFROM  IF DMINUS  ENDIF }
header number = {
	0x80 | 6    ,
	"NUMBER" ,
	0x80 ,
	&pnumber ,	  
	_number		};
_dashfind() { 
	BL WORD  HERE CONTEXT FETCH FETCH PFIND
	    DUP ZEROEQUALS IF  DROP HERE LATEST PFIND ENDIF }
header dashfind = {
	0x80 | 5    ,
	"-FIND" ,
	0x80 ,
	&number ,	  
	_dashfind		};
_pabort() { 
	ABORT }
header pabort = {
	0x80 | 7    ,
	"\050ABORT\051" ,
	0x80 ,
	&dashfind ,	  
	_pabort		};
header errornumber = {
	0x80 | 6    ,
	"ERROR#" ,
	0x80 ,
	&pabort ,	  
	docon ,
	(cell) (cell) &errno    	
};
_error() { 
	WARNING FETCH  ZEROLESS IF  PABORT  ENDIF  HERE COUNT TYPE
	    DOTQUOTE("  ? ") MESSAGE  SPSTORE  IN FETCH  BLK FETCH QUIT }
header error = {
	0x80 | 5    ,
	"ERROR" ,
	0x80 ,
	&errornumber ,	  
	_error		};

