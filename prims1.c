
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
#include "prims1.h"
_lit() { 
	push(*(cell *)ip); 
	(cell *) ip += 1; 
}
header lit = {
	0x80 | 3    ,
	"LIT" ,
	0x80 ,
	NULL ,	  
	_lit		};
_execute() { 
	w = (cell *) pop;
#if PCC
	(**w)();
#else
	(*(void(*)())*w)();
#endif
}
header execute = {
	0x80 | 7    ,
	"EXECUTE" ,
	0x80 ,
	&lit ,	  
	_execute		};
_branch() { 
	ip += *(cell *)ip; 
}
header branch = {
	0x80 | 6    ,
	"BRANCH" ,
	0x80 ,
	&execute ,	  
	_branch		};
_zerobranch() { 
	ip += pop ? sizeof(a_cell) : *(cell *)ip ; 
}
header zerobranch = {
	0x80 | 7    ,
	"0BRANCH" ,
	0x80 ,
	&branch ,	  
	_zerobranch		};
_ploop() { 
	if(++(*rsp) < *(rsp+1))
		ip += *(cell *)ip;
	else	{
		ip += sizeof(a_cell);
		rsp += 2;
	}
}
header ploop = {
	0x80 | 6    ,
	"\050LOOP\051" ,
	0x80 ,
	&zerobranch ,	  
	_ploop		};
_pplusloop() { 
	if((*dsp >= 0) ? (*rsp += pop) < *(rsp + 1): (*rsp += pop) > *(rsp + 1))
		ip += *(cell *)ip;
	else	{
		ip += sizeof(a_cell);
		rsp += 2;
	}
}
header pplusloop = {
	0x80 | 7    ,
	"\050+LOOP\051" ,
	0x80 ,
	&ploop ,	  
	_pplusloop		};
_pdo() { 
	rpush(*(dsp + 1)); 
	rpush(pop); 
	dsp++; 
}
header pdo = {
	0x80 | 4    ,
	"\050DO\051" ,
	0x80 ,
	&pplusloop ,	  
	_pdo		};
_i() { 
	push(*rsp); 
}
header i = {
	0x80 | 1    ,
	"I" ,
	0x80 ,
	&pdo ,	  
	_i		};
_digit() { 
	register unsigned int b = (int) pop;
	register int c = (int) pop - 0x30 /* adjust by subtracting ascii 0 */; 
	if(c < 0)	goto bad;
	if(c < 10)	goto ok;
	c &= 0xdf;	/* convert to upper case */
	c -= 7;		/* adjust to letters */
	if(c < 10)	goto bad;
ok:	
	if(c >= b)	goto bad;
	push(c);
	push(1);
	return;
bad:	
	push(0);
}
header digit = {
	0x80 | 5    ,
	"DIGIT" ,
	0x80 ,
	&i ,	  
	_digit		};
/* align n on machine boundry (see fsys.c for function) */
_align() { 
	register cell i = pop;
	push(sysalign(i)); 
}
header align = {
	0x80 | 5    ,
	"ALIGN" ,
	0x80 ,
	&digit ,	  
	_align		};
_pfind() { 
	register char *h, *n;	/* in order of importance for register assignments */
	register char c, diff;
	register char *norg, *horg;
	register char count;
	norg = (char *) pop;
	horg = (char *) pop;
	count = *horg;
	do	{
		h = horg;
		n = norg;
		c = count;
		diff = TRUE;
		if((*n & (char)0x3f) == c)	{	/* compare counts */
			do	{
				n++;
				h++;	/* move to next char */
				/* diff=comparison less msbit
									loop until words !same,
									count expires, or msbit set */
				diff = (*n & (char) 0x7f) - *h;
			}	
			while(!(--c == 0 || diff || *n & (char) 0x80));
		}
		else	n++;	/* counts not same, move to name */
		while(!(*n++ & (char)0x80));	/* skip rest of name */
		n = (char *) sysalign(n);
		if(diff == 0)	{
			push(n + (sizeof(a_cell) * 2));
			push((unsigned char) ((header *) norg)->length);
			push(1);
			return;	/* found */
		}
	}	
	while(norg =(char *) *(cell *)n);	/* follow link */
	push(FALSE);	/* not found */
}
header pfind = {
	0x80 | 6    ,
	"\050FIND\051" ,
	0x80 ,
	&align ,	  
	_pfind		};
_enclose() { 
	register char chr, *addr;
	register int c, fc, del, nc, ew;
	c = -1;		/* initialize offset count */
	del = pop;
	addr = (char *) pop;
	while(*(addr + ++c) == del);	/* skip leading delimiters */
	fc = c;
	do	{
		chr = *(addr + c);
		if(chr == NULL)	{	/* unconditional delimiter */
			nc = ew = c;
			if(fc == c)	ew++;
			break;
		}
		ew = c++;
		nc = c;
	}	
	while(chr != del);
	push(addr);
	push(fc);
	push(ew);
	push(nc);	/* next char */
}
header enclose = {
	0x80 | 7    ,
	"ENCLOSE" ,
	0x80 ,
	&pfind ,	  
	_enclose		};

