
 
/*
 *
 *		HenceFORTH in C		Version %I%
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
 */
/* Dictionary Header Structure */
typedef struct of_head	{
	char		length;		/* word length byte */
	char		word[HEADMAX-1]; /* text of word */
	char		terminal;	/* byte w/msbit set */
	struct of_head	*link;		/* to previous word */
	void		(*cf)();	/* pointer to some real code */
	cell		pf;		/* parameter field */
	cell		pf2;
	cell		pf3;
	cell		pf4;
	cell		pf5;
}	header, *to_head;

