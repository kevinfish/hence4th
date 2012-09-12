
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
extern header error;
_iddot() { 
	PAD LIT(0X20) LIT(0X5F) FILL  DUP PFA LFA OVER SUBTRACT
	    OVER CFETCH LIT(0X1F) AND ONEPLUS MIN
	    PAD SWAP CMOVE PAD COUNT LIT(0X1F) AND OVER OVER PLUS 
	    ONESUBTRACT DUP CFETCH  LIT(0X7F) AND SWAP CSTORE  TYPE SPACE }
header iddot = {
	0x80 | 3    ,
	"ID." ,
	0x80 ,
	&error ,	  
	_iddot		};
_create() { 
	FIRST HERE LIT(0XA0) PLUS TOR ZERO RFROM ZERO DMINUS
	    DPLUS SWAP DROP  TWO QERROR
	    DASHFIND IF  DROP NFA IDDOT  LIT(4) MESSAGE  SPACE ENDIF
	    HERE DUP CFETCH WIDTH FETCH  MIN ONEPLUS ALIGN ALLOT
	    DUP LIT(0XA0) TOGGLE  HERE ONE SUBTRACT
	    LIT(0X80) TOGGLE  LATEST COMMA
	    CURRENT FETCH STORE  HERE LIT(sizeof(a_cell)) PLUS COMMA }
header create = {
	0x80 | 6    ,
	"CREATE" ,
	0x80 ,
	&iddot ,	  
	_create		};
_bracketcompile() { 
	DASHFIND ZEROEQUALS ZERO QERROR
	    DROP CFA COMMA }
header bracketcompile = {
	0x80 | 9   | 0x40 ,
	"[COMPILE]" ,
	0x80 ,
	&create ,	  
	_bracketcompile		};
_literal() { 
	extern header lit;
	STATE FETCH IF  COMPILE(lit) COMMA  ENDIF
}
header literal = {
	0x80 | 7   | 0x40 ,
	"LITERAL" ,
	0x80 ,
	&bracketcompile ,	  
	_literal		};
_dliteral() { 
	STATE FETCH IF  SWAP  /*[COMPILE]*/ LITERAL
	    /*[COMPILE]*/ LITERAL ENDIF }
header dliteral = {
	0x80 | 8   | 0x40 ,
	"DLITERAL" ,
	0x80 ,
	&literal ,	  
	_dliteral		};
_qstack() { 
	LIT(datastack + DSMAX - 1) SPFETCH LESS ONE QERROR
	    SPFETCH LIT(datastack + 1) LESS LIT(7) QERROR }
header qstack = {
	0x80 | 6    ,
	"?STACK" ,
	0x80 ,
	&dliteral ,	  
	_qstack		};
_interpret() { 
	w = (cell *)&dummyi - 1;  
	docolon(); 
}
header interpret = {
	0x80 | 9    ,
	"INTERPRET" ,
	0x80 ,
	&qstack ,	  
	_interpret		};
_immediate() { 
	LATEST LIT(0X40) TOGGLE }
header immediate = {
	0x80 | 9    ,
	"IMMEDIATE" ,
	0x80 ,
	&interpret ,	  
	_immediate		};
_vocabulary() { 
	BUILDS LIT(FAKE) COMMA  CURRENT FETCH  CFA COMMA
	    HERE VOCLINK FETCH COMMA VOCLINK STORE DOES(&dummy2) }
header vocabulary = {
	0x80 | 10    ,
	"VOCABULARY" ,
	0x80 ,
	&immediate ,	  
	_vocabulary		};
header forth = {
	0x80 | 5   | 0x40 ,
	"FORTH" ,
	0x80 ,
	&vocabulary ,	  
	dodoes ,
	(cell) &dummy2  ,
	(cell) FAKE  ,
	(cell) 0  ,
	(cell) 0 	
};
_definitions() { 
	CONTEXT FETCH  CURRENT STORE }
header definitions = {
	0x80 | 11    ,
	"DEFINITIONS" ,
	0x80 ,
	&forth ,	  
	_definitions		};
_paren() { 
	LIT(0X29) WORD }
header paren = {
	0x80 | 1   | 0x40 ,
	"\050" ,
	0x80 ,
	&definitions ,	  
	_paren		};
_quit() { 
	longjmp(to_quit,1); 
}
header quit = {
	0x80 | 4    ,
	"QUIT" ,
	0x80 ,
	&paren ,	  
	_quit		};
_abort() { 
	longjmp(to_abort,1); 
}
header abort = {
	0x80 | 5    ,
	"ABORT" ,
	0x80 ,
	&quit ,	  
	_abort		};
_warm() { 
	longjmp(to_warm,1); 
}
header warm = {
	0x80 | 4    ,
	"WARM" ,
	0x80 ,
	&abort ,	  
	_warm		};
_cold() { 
	longjmp(to_cold,1); 
}
header cold = {
	0x80 | 4    ,
	"COLD" ,
	0x80 ,
	&warm ,	  
	_cold		};
_stod() { 
	if(*dsp < 0) push(-1); 
	else push(0); 
}
header stod = {
	0x80 | 4    ,
	"S->D" ,
	0x80 ,
	&cold ,	  
	_stod		};
_plusminus() { 
	ZEROLESS IF  MINUS  ENDIF }
header plusminus = {
	0x80 | 2    ,
	"+-" ,
	0x80 ,
	&stod ,	  
	_plusminus		};
_dplusminus() { 
	ZEROLESS IF  DMINUS  ENDIF }
header dplusminus = {
	0x80 | 3    ,
	"D+-" ,
	0x80 ,
	&plusminus ,	  
	_dplusminus		};
_abss() { 
	DUP PLUSMINUS }
header abss = {
	0x80 | 3    ,
	"ABS" ,
	0x80 ,
	&dplusminus ,	  
	_abss		};
_dabs() { 
	DUP DPLUSMINUS }
header dabs = {
	0x80 | 4    ,
	"DABS" ,
	0x80 ,
	&abss ,	  
	_dabs		};
_min() { 
	OVER  OVER  GREATER IF  SWAP  ENDIF  DROP }
header min = {
	0x80 | 3    ,
	"MIN" ,
	0x80 ,
	&dabs ,	  
	_min		};
_max() { 
	OVER  OVER  LESS IF  SWAP  ENDIF  DROP }
header max = {
	0x80 | 3    ,
	"MAX" ,
	0x80 ,
	&min ,	  
	_max		};
_mstar() { 
	OVER OVER XOR TOR  ABSS  SWAP  ABSS USTAR  RFROM DPLUSMINUS }
header mstar = {
	0x80 | 2    ,
	"M*" ,
	0x80 ,
	&max ,	  
	_mstar		};
_mslash() { 
	OVER TOR TOR  DABS  R  ABSS  USLASH
	    RFROM  R  XOR  PLUSMINUS  SWAP  RFROM  PLUSMINUS  SWAP }
header mslash = {
	0x80 | 2    ,
	"M/" ,
	0x80 ,
	&mstar ,	  
	_mslash		};
_star() { 
	USTAR DROP }
header star = {
	0x80 | 1    ,
	"*" ,
	0x80 ,
	&mslash ,	  
	_star		};
_slashmod() { 
	TOR  STOD  RFROM  MSLASH }
header slashmod = {
	0x80 | 4    ,
	"/MOD" ,
	0x80 ,
	&star ,	  
	_slashmod		};
_slash() { 
	SLASHMOD  SWAP  DROP }
header slash = {
	0x80 | 1    ,
	"/" ,
	0x80 ,
	&slashmod ,	  
	_slash		};
_mod() { 
	SLASHMOD  DROP }
header mod = {
	0x80 | 3    ,
	"MOD" ,
	0x80 ,
	&slash ,	  
	_mod		};
_ssmod() { 
	TOR  MSTAR  RFROM  MSLASH }
header ssmod = {
	0x80 | 5    ,
	"*/MOD" ,
	0x80 ,
	&mod ,	  
	_ssmod		};
_starslash() { 
	SSMOD  SWAP  DROP }
header starslash = {
	0x80 | 2    ,
	"*/" ,
	0x80 ,
	&ssmod ,	  
	_starslash		};
_msmod() { 
	TOR  ZERO R USLASH  RFROM SWAP TOR  USLASH  RFROM }
header msmod = {
	0x80 | 5    ,
	"M/MOD" ,
	0x80 ,
	&starslash ,	  
	_msmod		};

