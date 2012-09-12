 
/*
 *
 *	HenceFORTH in C    Ver 1.2
 *	(c) Copyright 1990  MISSING LINK
 *	All Rights Reserved
 *
 *	File:	declare.h
 *	Desc:	Global declaration for the stuff in misc.c
 *
 */
 
typedef int cell;
typedef int a_cell;
typedef unsigned int u_cell;

extern void next(), docolon(), docon(), dovar(), douser(), dodoes();
extern void dovoc(), doquit(), doabort(), start(), dowarm(), docold();
extern void goodbye(), dointerpret(), initsys();
 
extern cell datastack[], *dsp, retstack[], *rsp, origin[], *w,
	*dummy2, *dummyi;
 
extern jmp_buf to_cold, to_warm, to_abort, to_quit, jmpstack[], *jsp;
 
extern char memory[], *ip, *up, *sysalign();
 
extern int fhandle[], errno, getkey();
 

