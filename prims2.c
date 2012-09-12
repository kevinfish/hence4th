
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
#include "prims2.h"
extern header enclose;
_emit() { 
	extern header out;
	(*(cell *)(up + out.pf))++; 
	putchar(pop);
}
header emit = {
	0x80 | 4    ,
	"EMIT" ,
	0x80 ,
	&enclose ,	  
	_emit		};
_key() { 
	BEGIN  QTERMINAL  DASHDUP  UNTIL }
header key = {
	0x80 | 3    ,
	"KEY" ,
	0x80 ,
	&emit ,	  
	_key		};
_qterminal() { 
	push(getkey()); 
}
header qterminal = {
	0x80 | 9    ,
	"?TERMINAL" ,
	0x80 ,
	&key ,	  
	_qterminal		};
_cr() { 
	LIT(13) EMIT LIT(10) EMIT }
header cr = {
	0x80 | 2    ,
	"CR" ,
	0x80 ,
	&qterminal ,	  
	_cr		};
_cmove() { 
	register char *from, *to;
	register int count;
	count = pop;
	to = (char *) pop;
	from = (char *) pop;
	while(count--) *to++ = *from++;
}
header cmove = {
	0x80 | 5    ,
	"CMOVE" ,
	0x80 ,
	&cr ,	  
	_cmove		};
_ustar() { 
#if SHORTCELL
	register u_cell i = pop;
	long ans = (u_cell) pop * (long) i;
	push(ans);
	push(HI(ans));
#else
	/*
		   ab
		   cd
		=====
		  efg
		 hij0
		=====
		*/
	u_cell tmp = pop;
	unsigned short a,b,c,d,e,f,g,h,i,j,carry;
	a = HI(tmp); 
	b = LO(tmp);
	tmp = pop; 
	c = HI(tmp); 
	d = LO(tmp);
	tmp = d * b; 
	g = LO(tmp); 
	carry = HI(tmp);
	tmp = a * d + carry; 
	f = LO(tmp); 
	e = HI(tmp);
	tmp = c * b; 
	j = LO(tmp); 
	carry = HI(tmp);
	tmp = c * a + carry; 
	i = LO(tmp); 
	h = HI(tmp);
	tmp = f + j; 
	j = LO(tmp); 
	carry = HI(tmp);
	tmp = e + i + carry; 
	i = LO(tmp); 
	carry = HI(tmp);
	h += carry;
	push(SHIFTUP(j) | (unsigned long) g);
	push(SHIFTUP(h) | (unsigned long) i);
#endif
}
header ustar = {
	0x80 | 2    ,
	"U*" ,
	0x80 ,
	&cmove ,	  
	_ustar		};
_uslash() { 
#if SHORTCELL
	register u_cell divisor = pop;
	register u_cell hi = pop;
	register long dividend = (u_cell) pop | SHIFTUP(hi);
	push(dividend % divisor);	/* push remainder */
	push(dividend / divisor);	/* and quotent */
#else
	unsigned short *i,dvdend[4],carry,ocarry;
	u_cell quotent = 0;
	unsigned short count = 32;
	u_cell divisor = pop;
	u_cell tmp = pop;
	dvdend[3] = HI(tmp);
	dvdend[2] = LO(tmp);
	tmp = pop;
	dvdend[1] = HI(tmp);
	dvdend[0] = LO(tmp);
	do {
		/* shift double len integer left one bit */
		ocarry = 0;
		for(i = dvdend; i <= &dvdend[3]; i++)	{
			carry = ((*i & (short) 0x8000) != 0);
			*i = (*i << 1) | ocarry;
			ocarry = carry;
		}
		quotent <<= 1;
		tmp = SHIFTUP(dvdend[3]) | dvdend[2];
		if(divisor <= tmp)	{
			tmp -= divisor;
			dvdend[3] = HI(tmp);
			dvdend[2] = LO(tmp);
			quotent++;
		}
	} 
	while( --count );
	push(tmp);	/* remainder */
	push(quotent);
#endif
}
header uslash = {
	0x80 | 2    ,
	"U/" ,
	0x80 ,
	&ustar ,	  
	_uslash		};
_and() { 
	*(dsp + 1) &= *(dsp); 
	dsp++; 
}
header and = {
	0x80 | 3    ,
	"AND" ,
	0x80 ,
	&uslash ,	  
	_and		};
_or() { 
	*(dsp + 1) |= *(dsp); 
	dsp++; 
}
header or = {
	0x80 | 2    ,
	"OR" ,
	0x80 ,
	&and ,	  
	_or		};
_xor() { 
	*(dsp + 1) ^= *(dsp); 
	dsp++; 
}
header xor = {
	0x80 | 3    ,
	"XOR" ,
	0x80 ,
	&or ,	  
	_xor		};
_spfetch() { 
	register cell *i = dsp;
	push(i);
}
header spfetch = {
	0x80 | 3    ,
	"SP@" ,
	0x80 ,
	&xor ,	  
	_spfetch		};
_spstore() { 
	extern header szero;
	dsp = (cell *) *(cell *)(up + szero.pf);
}
header spstore = {
	0x80 | 3    ,
	"SP!" ,
	0x80 ,
	&spfetch ,	  
	_spstore		};
_rpstore() { 
	extern header rzero;
	rsp = (cell *) *(cell *)(up + rzero.pf);
	jsp = jmpstack + RSMAX;
}
header rpstore = {
	0x80 | 3    ,
	"RP!" ,
	0x80 ,
	&spstore ,	  
	_rpstore		};
_semis() { 
	register i = rpop;
	if(i)  	 ip = (char *) i;
	longjmp(*jsp++, 1);
}
header semis = {
	0x80 | 2    ,
	";S" ,
	0x80 ,
	&rpstore ,	  
	_semis		};
_leave() { 
	*(rsp + 1) = *rsp; 
}
header leave = {
	0x80 | 5    ,
	"LEAVE" ,
	0x80 ,
	&semis ,	  
	_leave		};
_tor() { 
	rpush(pop); 
	--jsp; 
}
header tor = {
	0x80 | 2    ,
	">R" ,
	0x80 ,
	&leave ,	  
	_tor		};
_rfrom() { 
	push(rpop); 
	jsp++; 
}
header rfrom = {
	0x80 | 2    ,
	"R>" ,
	0x80 ,
	&tor ,	  
	_rfrom		};
_r() { 
	push(*rsp); 
}
header r = {
	0x80 | 1    ,
	"R" ,
	0x80 ,
	&rfrom ,	  
	_r		};
_zeroequals() { 
	*(dsp) = (*(dsp) == 0); 
}
header zeroequals = {
	0x80 | 2    ,
	"0=" ,
	0x80 ,
	&r ,	  
	_zeroequals		};
_zeroless() { 
	*(dsp) = (*(dsp) < 0); 
}
header zeroless = {
	0x80 | 2    ,
	"0<" ,
	0x80 ,
	&zeroequals ,	  
	_zeroless		};
_plus() { 
	*(dsp + 1) += *(dsp); 
	dsp++; 
}
header plus = {
	0x80 | 1    ,
	"+" ,
	0x80 ,
	&zeroless ,	  
	_plus		};
_dplus() { 
#if SHORTCELL
	register cell hi = pop;
	long i = (u_cell) pop | SHIFTUP(hi);
	hi = pop;
	i += (u_cell) pop | SHIFTUP(hi);
	push(LO(i));
	push(HI(i));
#else
	cell tmp;
	short i, carry = 0;
	unsigned short n[8];
	for(i = 0; i < 8; i += 2)	{
		tmp = pop;
		n[i] = HI(tmp);
		n[i + 1] = LO(tmp);
	}
	for(i = 3; i >= 0; i--)	{
		tmp = carry + n[i] + n[i + 4];
		carry = HI(tmp);
		n[i] = LO(tmp);
	}
	push(SHIFTUP(n[2]) | n[3]);
	push(SHIFTUP(n[0]) | n[1]);
#endif
}
header dplus = {
	0x80 | 2    ,
	"D+" ,
	0x80 ,
	&plus ,	  
	_dplus		};
_minus() { 
	*(dsp) = ~*(dsp) + 1; 
}
header minus = {
	0x80 | 5    ,
	"MINUS" ,
	0x80 ,
	&dplus ,	  
	_minus		};
_dminus() { 
#if SHORTCELL
	register cell hi = pop;
	long i = (u_cell) pop | SHIFTUP(hi);
	i *= -1;
	push(LO(i));
	push(HI(i));
#else
	u_cell tmp, j = 0;
	unsigned short *i, carry = 1, n[4];
	tmp = ~pop;
	n[3] = HI(tmp);
	n[2] = LO(tmp);
	tmp = ~pop;
	n[1] = HI(tmp);
	n[0] = LO(tmp);
	for(i = n; i < &n[4]; i++)	{
		j = *i + carry;
		carry = HI(j);
		*i = LO(j);
	}
	push(SHIFTUP(n[1]) | n[0]);
	push(SHIFTUP(n[3]) | n[2]);
#endif
}
header dminus = {
	0x80 | 6    ,
	"DMINUS" ,
	0x80 ,
	&minus ,	  
	_dminus		};
_over() { 
	*(dsp - 1) = *(dsp + 1); 
	dsp--; 
}
header over = {
	0x80 | 4    ,
	"OVER" ,
	0x80 ,
	&dminus ,	  
	_over		};
_drop() { 
	pop; 
}
header drop = {
	0x80 | 4    ,
	"DROP" ,
	0x80 ,
	&over ,	  
	_drop		};
_swap() { 
	register cell i = *(dsp);
	*(dsp) = *(dsp + 1);
	*(dsp + 1) = i;
}
header swap = {
	0x80 | 4    ,
	"SWAP" ,
	0x80 ,
	&drop ,	  
	_swap		};
_dup() { 
	register cell i = *dsp;
	push(i);
}
header dup = {
	0x80 | 3    ,
	"DUP" ,
	0x80 ,
	&swap ,	  
	_dup		};
_plusstore() { 
	register cell *mem = (cell *) pop;  
	*mem += pop; 
}
header plusstore = {
	0x80 | 2    ,
	"+!" ,
	0x80 ,
	&dup ,	  
	_plusstore		};
_toggle() { 
	register char mask = pop; 
	*(char *)pop ^= mask; 
}
header toggle = {
	0x80 | 6    ,
	"TOGGLE" ,
	0x80 ,
	&plusstore ,	  
	_toggle		};
_fetch() { 
	*dsp = *(cell *) *dsp; 
}
header fetch = {
	0x80 | 1    ,
	"@" ,
	0x80 ,
	&toggle ,	  
	_fetch		};
_wfetch() { 
	*dsp = (unsigned short) *(short *) *dsp; 
}
header wfetch = {
	0x80 | 2    ,
	"W@" ,
	0x80 ,
	&fetch ,	  
	_wfetch		};
_cfetch() { 
	*dsp = (unsigned char) *(char *) *dsp; 
}
header cfetch = {
	0x80 | 2    ,
	"C@" ,
	0x80 ,
	&wfetch ,	  
	_cfetch		};
_store() { 
	register cell *to = (cell *)pop; 
	*to = pop; 
}
header store = {
	0x80 | 1    ,
	"!" ,
	0x80 ,
	&cfetch ,	  
	_store		};
_wstore() { 
	register short *to = (short *)pop; 
	*to = pop; 
}
header wstore = {
	0x80 | 2    ,
	"W!" ,
	0x80 ,
	&store ,	  
	_wstore		};
_cstore() { 
	register char *to = (char *)pop; 
	*to = pop; 
}
header cstore = {
	0x80 | 2    ,
	"C!" ,
	0x80 ,
	&wstore ,	  
	_cstore		};

