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
 */

#define I8088	TRUE	/* define microprocessor type */
#define M68K	FALSE	/* */
#define Z80	FALSE	/* */
#define M6809	FALSE	/* */


#define JMP	0	/* for total compatibility your machines word length
				JMP opcode (might need preceeding NOP) */


#define TOPWORD bye	/* last word in FORTH vocabulary */


#define HEADMAX (15)	/* Max width of header in kernel */


#define UNIX	FALSE	/* define if a unix implementation */
#define DOS	TRUE	/* define if a dos/cpm based implementation */


#if I8088 || Z80
# define SWAPPED TRUE	/* define if little endian(e.g. intel/zilog type ) */
#else
# define SWAPPED FALSE
#endif


#if M68K
# define WORD_ALIGNED	TRUE
#else
# define WORD_ALIGNED	FALSE
#endif


#define ATT	FALSE
#define BSD	FALSE	/* */

#if DOS
#define EOL '\r'	/* systems End Of Line char (e.g. RETURN) */
#elif UNIX
#define EOL '\n'
#endif

#define V7	FALSE	/* unix Version 7 compatiblity */
#define SYSV	FALSE
#define BSD2	FALSE	/* */
#define BSD4	FALSE	/* */


#define PCC	FALSE	/* Personal C compiler */
#define MIX	TRUE
#define MIXSM	TRUE	/* mix C compiler Small Model */
#define MIXMM	FALSE	/* Mix C compiler Medium Model */
#define MIXLM	FALSE	/* Mix C comipler Large Model */

#if PCC || MIXSM
# define SHORTCELL	TRUE	/* 16 bit pointers */
#else
# define SHORTCELL	FALSE
#endif


/* EOM and NBUFS are just suggestions for small and large machines. */
/* You can customize them to more correctly fit your memory model and */
/* application.  Remember that the disk buffers and user variables are */
/* alloted beyond the end of the dictionary.  Also the stacks and defaults */
/* are alloted before the dictionary.  This is why the small model leaves */
/* some extra room (e.g. 64k - 50k = 14k).  See the fig installation guide */
/* or any number of good texts on figFORTH */	

#if SHORTCELL

# define EOM (50*1024)	/* dictionary length */
# define NBUFS (4) /* number of buffers */

#else

# define EOM (128*1024)	/* dictionary length */
# define NBUFS (8)	/* number of buffers */

#endif


#define BPERDRIVE	1024	/* blocks per drive (or screen file) */


#define DSMAX (50)	/* maximum size of data stack */
#define RSMAX (50)	/*    "     "   "  return stack */


#define TIBSIZE (128)	/* terminal input buffer size */
#define CPERL (64)	/* chars per line (do not change unless sure) */


#define BPERBUF (1024)		/* bytes per buffer */
#define BPERSCR (1024/BPERBUF)	/* blocks per screen */
#define FIRSTBUF (EOM-(NBUFS*(BPERBUF+sizeof(a_cell) * 2))) /* alignment? */


#define HI(x)	(short) ((x) >> 16)	/* return high 16 bit int*/
#define LO(x)	((short) x)	/* return low 16 bits */
#define SHIFTUP(x) (((long) x) << 16)


/*	determine buffer update flag */
#if	SHORTCELL
# define UFLAG	(0x8000)
#else
# define UFLAG	(0x80000000L)
#endif


/*	setup fake head for vocabularies 
 *
 *	This should be different for different machines.
 *	See a good discussion on the fig VOCABULARY structure
 *	(as found in "The Forth Encyclopedia") if you have difficulty.
 */

#if SHORTCELL
# if SWAPPED
#  define FAKE	(0xA081)
# else
#  define FAKE	(0x81A0)
# endif
#else
# if SWAPPED
#  define FAKE	(0xA0202083)	/* this might be right for intel */
# else
#  define FAKE	(0x832020A0)
# endif
#endif

