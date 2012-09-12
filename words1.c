
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
#include "words1.h"
extern header cstore;
_colon() { 
	QEXEC STORECSP CURRENT FETCH CONTEXT STORE
	    CREATE RIGHTBRACKET SEMICODE(docolon) }
header colon = {
	0x80 | 1    ,
	":" ,
	0x80 ,
	&cstore ,	  
	_colon		};
_semi() { 
	extern header semis;
	QCSP COMPILE(semis) SMUDGE /*[COMPILE]*/ LEFTBRACKET
}
header semi = {
	0x80 | 1   | 0x40 ,
	";" ,
	0x80 ,
	&colon ,	  
	_semi		};
_constant() { 
	CREATE SMUDGE COMMA SEMICODE(docon) }
header constant = {
	0x80 | 8    ,
	"CONSTANT" ,
	0x80 ,
	&semi ,	  
	_constant		};
_variable() { 
	CONSTANT SEMICODE(dovar) }
header variable = {
	0x80 | 8    ,
	"VARIABLE" ,
	0x80 ,
	&constant ,	  
	_variable		};
_user() { 
	CONSTANT SEMICODE(douser) }
header user = {
	0x80 | 4    ,
	"USER" ,
	0x80 ,
	&variable ,	  
	_user		};
header zero = {
	0x80 | 1    ,
	"0" ,
	0x80 ,
	&user ,	  
	docon ,
	(cell) 0    	
};
header one = {
	0x80 | 1    ,
	"1" ,
	0x80 ,
	&zero ,	  
	docon ,
	(cell) 1    	
};
header two = {
	0x80 | 1    ,
	"2" ,
	0x80 ,
	&one ,	  
	docon ,
	(cell) 2    	
};
header three = {
	0x80 | 1    ,
	"3" ,
	0x80 ,
	&two ,	  
	docon ,
	(cell) 3    	
};
header bl = {
	0x80 | 2    ,
	"BL" ,
	0x80 ,
	&three ,	  
	docon ,
	(cell) 32    	
};
header cslashl = {
	0x80 | 3    ,
	"C/L" ,
	0x80 ,
	&bl ,	  
	docon ,
	(cell) CPERL    	
};
header first = {
	0x80 | 5    ,
	"FIRST" ,
	0x80 ,
	&cslashl ,	  
	docon ,
#if MIX
	(cell) memory+FIRSTBUF    	
#else
	(cell) (memory+FIRSTBUF)
#endif
};
header limit = {
	0x80 | 5    ,
	"LIMIT" ,
	0x80 ,
	&first ,	  
	docon ,
#if MIX
	(cell) memory+EOM    	
#else
	(cell) (memory+EOM)
#endif
};
header bslashbuf = {
	0x80 | 5    ,
	"B/BUF" ,
	0x80 ,
	&limit ,	  
	docon ,
	(cell) BPERBUF    	
};
header bslashscr = {
	0x80 | 5    ,
	"B/SCR" ,
	0x80 ,
	&bslashbuf ,	  
	docon ,
	(cell) 1024/BPERBUF    	
};
header numberbuf = {
	0x80 | 4    ,
	"#BUF" ,
	0x80 ,
	&bslashscr ,	  
	docon ,
	(cell) NBUFS    	
};
_plusorigin() { 
	register cell i = pop;
#if !SHORTCELL
	i *= 2;  /* fake this out for compatibility */
#endif
	push((char *)origin + i);
}
header plusorigin = {
	0x80 | 7    ,
	"+ORIGIN" ,
	0x80 ,
	&numberbuf ,	  
	_plusorigin		};
header szero = {
	0x80 | 2    ,
	"S0" ,
	0x80 ,
	&plusorigin ,	  
	douser ,
	(cell) 3 * sizeof(a_cell)    	
};
/* stack origin */
header rzero = {
	0x80 | 2    ,
	"R0" ,
	0x80 ,
	&szero ,	  
	douser ,
	(cell) 4 * sizeof(a_cell)    	
};
header tib = {
	0x80 | 3    ,
	"TIB" ,
	0x80 ,
	&rzero ,	  
	douser ,
	(cell) 5 * sizeof(a_cell)    	
};
header width = {
	0x80 | 5    ,
	"WIDTH" ,
	0x80 ,
	&tib ,	  
	douser ,
	(cell) 6 * sizeof(a_cell)    	
};
header warning = {
	0x80 | 7    ,
	"WARNING" ,
	0x80 ,
	&width ,	  
	douser ,
	(cell) 7 * sizeof(a_cell)    	
};
header fence = {
	0x80 | 5    ,
	"FENCE" ,
	0x80 ,
	&warning ,	  
	douser ,
	(cell) 8 * sizeof(a_cell)    	
};
header dp = {
	0x80 | 2    ,
	"DP" ,
	0x80 ,
	&fence ,	  
	douser ,
	(cell) 9 * sizeof(a_cell)    	
};
header voclink = {
	0x80 | 8    ,
	"VOC-LINK" ,
	0x80 ,
	&dp ,	  
	douser ,
	(cell) 10 * sizeof(a_cell)    	
};
header blk = {
	0x80 | 3    ,
	"BLK" ,
	0x80 ,
	&voclink ,	  
	douser ,
	(cell) 11 * sizeof(a_cell)    	
};
header in = {
	0x80 | 2    ,
	"IN" ,
	0x80 ,
	&blk ,	  
	douser ,
	(cell) 12 * sizeof(a_cell)    	
};
header out = {
	0x80 | 3    ,
	"OUT" ,
	0x80 ,
	&in ,	  
	douser ,
	(cell) 13 * sizeof(a_cell)    	
};
header scr = {
	0x80 | 3    ,
	"SCR" ,
	0x80 ,
	&out ,	  
	douser ,
	(cell) 14 * sizeof(a_cell)    	
};
header offset = {
	0x80 | 6    ,
	"OFFSET" ,
	0x80 ,
	&scr ,	  
	douser ,
	(cell) 15 * sizeof(a_cell)    	
};
header context = {
	0x80 | 7    ,
	"CONTEXT" ,
	0x80 ,
	&offset ,	  
	douser ,
	(cell) 16 * sizeof(a_cell)    	
};
header current = {
	0x80 | 7    ,
	"CURRENT" ,
	0x80 ,
	&context ,	  
	douser ,
	(cell) 17 * sizeof(a_cell)    	
};
header state = {
	0x80 | 5    ,
	"STATE" ,
	0x80 ,
	&current ,	  
	douser ,
	(cell) 18 * sizeof(a_cell)    	
};
header base = {
	0x80 | 4    ,
	"BASE" ,
	0x80 ,
	&state ,	  
	douser ,
	(cell) 19 * sizeof(a_cell)    	
};
header dpl = {
	0x80 | 3    ,
	"DPL" ,
	0x80 ,
	&base ,	  
	douser ,
	(cell) 20 * sizeof(a_cell)    	
};
header fld = {
	0x80 | 3    ,
	"FLD" ,
	0x80 ,
	&dpl ,	  
	douser ,
	(cell) 21 * sizeof(a_cell)    	
};
header csp = {
	0x80 | 3    ,
	"CSP" ,
	0x80 ,
	&fld ,	  
	douser ,
	(cell) 22 * sizeof(a_cell)    	
};
header rsharp = {
	0x80 | 2    ,
	"R#" ,
	0x80 ,
	&csp ,	  
	douser ,
	(cell) 23 * sizeof(a_cell)    	
};
header hld = {
	0x80 | 3    ,
	"HLD" ,
	0x80 ,
	&rsharp ,	  
	douser ,
	(cell) 24 * sizeof(a_cell)    	
};
_onesubtract() { 
	(*dsp)--; 
}
header onesubtract = {
	0x80 | 2    ,
	"1-" ,
	0x80 ,
	&hld ,	  
	_onesubtract		};
_twosubtract() { 
	*dsp -= 2; 
}
header twosubtract = {
	0x80 | 2    ,
	"2-" ,
	0x80 ,
	&onesubtract ,	  
	_twosubtract		};
_threesubtract() { 
	*dsp -= 3; 
}
header threesubtract = {
	0x80 | 2    ,
	"3-" ,
	0x80 ,
	&twosubtract ,	  
	_threesubtract		};
_foursubtract() { 
	*dsp -= 4; 
}
header foursubtract = {
	0x80 | 2    ,
	"4-" ,
	0x80 ,
	&threesubtract ,	  
	_foursubtract		};
_oneplus() { 
	(*dsp)++; 
}
header oneplus = {
	0x80 | 2    ,
	"1+" ,
	0x80 ,
	&foursubtract ,	  
	_oneplus		};
_twoplus() { 
	*dsp += 2; 
}
header twoplus = {
	0x80 | 2    ,
	"2+" ,
	0x80 ,
	&oneplus ,	  
	_twoplus		};
_threeplus() { 
	*dsp += 3; 
}
header threeplus = {
	0x80 | 2    ,
	"3+" ,
	0x80 ,
	&twoplus ,	  
	_threeplus		};
_fourplus() { 
	*dsp += 4; 
}
header fourplus = {
	0x80 | 2    ,
	"4+" ,
	0x80 ,
	&threeplus ,	  
	_fourplus		};
_twostar() { 
	*dsp <<= 1; 
}
header twostar = {
	0x80 | 2    ,
	"2*" ,
	0x80 ,
	&fourplus ,	  
	_twostar		};
_twoslash() { 
	*dsp >>= 1; 
}
header twoslash = {
	0x80 | 2    ,
	"2/" ,
	0x80 ,
	&twostar ,	  
	_twoslash		};
_cells() { 
#if SHORTCELL
	*dsp *= 2;
#else
	*dsp *= 4;
#endif
}
header cells = {
	0x80 | 5    ,
	"CELLS" ,
	0x80 ,
	&twoslash ,	  
	_cells		};
_here() { 
	DP FETCH }
header here = {
	0x80 | 4    ,
	"HERE" ,
	0x80 ,
	&cells ,	  
	_here		};
_allot() { 
	DP PLUSSTORE }
header allot = {
	0x80 | 5    ,
	"ALLOT" ,
	0x80 ,
	&here ,	  
	_allot		};
_comma() { 
	HERE ALIGN DUP DP STORE  STORE LIT(sizeof(a_cell)) ALLOT }
header comma = {
	0x80 | 1    ,
	"\054" ,
	0x80 ,
	&allot ,	  
	_comma		};
_wcomma() { 
	HERE ALIGN DUP DP STORE
	    WSTORE LIT(sizeof(short)) ALLOT }
header wcomma = {
	0x80 | 2    ,
	"W\054" ,
	0x80 ,
	&comma ,	  
	_wcomma		};
_ccomma() { 
	HERE CSTORE ONE ALLOT }
header ccomma = {
	0x80 | 2    ,
	"C\054" ,
	0x80 ,
	&wcomma ,	  
	_ccomma		};
_subtract() { 
	MINUS PLUS }
header subtract = {
	0x80 | 1    ,
	"-" ,
	0x80 ,
	&ccomma ,	  
	_subtract		};
_equals() { 
	SUBTRACT ZEROEQUALS }
header equals = {
	0x80 | 1    ,
	"=" ,
	0x80 ,
	&subtract ,	  
	_equals		};
_less() { 
	SUBTRACT ZEROLESS }
header less = {
	0x80 | 1    ,
	"<" ,
	0x80 ,
	&equals ,	  
	_less		};
_greater() { 
	SWAP LESS }
header greater = {
	0x80 | 1    ,
	">" ,
	0x80 ,
	&less ,	  
	_greater		};
_rot() { 
	TOR SWAP RFROM SWAP }
header rot = {
	0x80 | 3    ,
	"ROT" ,
	0x80 ,
	&greater ,	  
	_rot		};
_space() { 
	BL EMIT }
header space = {
	0x80 | 5    ,
	"SPACE" ,
	0x80 ,
	&rot ,	  
	_space		};
_dashdup() { 
	DUP IF DUP ENDIF }
header dashdup = {
	0x80 | 4    ,
	"-DUP" ,
	0x80 ,
	&space ,	  
	_dashdup		};

