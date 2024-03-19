/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  U    U             CCCC                          l      (C) Copyright   */
/*  U    U            C                              l       UniComal A/S   */
/*  U    U  nnnn   i  C       ooo   mmmmmm    aaaa   l          1990        */
/*  U    U  n   n  i  C      o   o  m  m  m  a   a   l                      */
/*  U    U  n   n  i  C      o   o  m  m  m  a   a   l       C - C O M A L  */
/*   UUUU   n   n  i   CCCC   ooo   m  m  m   aaaaa   ll    =============== */
/*                                                                          */
/*----------------------------------------------------------------------------

  File         : comal.h
  Version      : 1.20
  programmer   : adj
  date         : 10 March 1990
  requirements :

  description  : Header file for Comal modules written in MS-C.

                 It includes the necessary types, externals, and
                 defines needed for coding modules for Comal in the
                 C language.

  revisions    : 10 March   1990, LNL; Written for DOS.
                 26 July    1990, LNL; Adapted for OS/2.
                 22 January 1992, LNL; The INT symbol left out qua OS/2 conflict

----------------------------------------------------------------------------*/

/* Comal style string */

#define STR_DEF(max) struct \
{ \
     unsigned short dim_length;         /* Dimensioned length */ \
     unsigned short cur_length;         /* Current length     */ \
     unsigned char  string[max];        /* Character sequence */ \
}

typedef STR_DEF(1) COMAL_STRING;

/* Comal style arrays */

typedef struct                          /* Index space record */
{
     signed   short ilow;               /* Lowbound(negated) (-32767..32767) */
     unsigned short ilen;               /* Length of index space (0..65535) */
} INDEX_SPACE;

/* Array dope vector layout */

typedef struct
{
     void far *base;          /* Address of first array component */
     unsigned short waste;    /* No. of bytes wasted before array - for alignment! */
     void near *ibase;        /* Element base in first segment (used in OS/2 only) */
     unsigned short isize;    /* Elements per segment (used in OS/2 only) */
     unsigned short size;     /* Size of each element (always even) */
     unsigned short dim4;     /* Dimension*4 */
     INDEX_SPACE idx[1];      /* Index spaces */
} ARRAY_DOPE;


/* Event message buffer */

typedef struct event_entry EVENT_ENTRY;

struct event_entry
{
     EVENT_ENTRY far *next_event;       /* Link to next event */
     unsigned short priority;           /* Priority of this event */
     short proc_handle[4];              /* Descriptor of PROCedure to execute */
};

/*#define INT         long */           /* Comal's integer type */
#define REAL        double              /* Comal's floating point type */
#define STR         COMAL_STRING far    /* Comal's string type */
#define INT_ARRAY   ARRAY_DOPE far      /* Comal's integer array type */
#define REAL_ARRAY  ARRAY_DOPE far      /* Comal's real array type */
#define STR_ARRAY   ARRAY_DOPE far      /* Comal's string array type */
#define PTR_ARRAY   ARRAY_DOPE far      /* Comal's pointer array type */
#define STRUC_ARRAY ARRAY_DOPE far      /* Comal's struc array type */

/* Signal types */

#define S_LOAD          0     /* Issued during CHAIN, RUN, SCAN */
#define S_DISCARD       2     /* Issued by NEW, LOAD, CHAIN, BYE(, RUN, SCAN) */
#define S_LINK          4     /* Issued during RUN, CHAIN */
#define S_UNLINK        6     /* Issued before SCAN, RUN, CHAIN, NEW, BYE */
#define S_STOP          8     /* Issued by STOP, END or Ctrl-Break */
#define S_CON          10     /* Issued by CON */
#define S_ERROR_1      12     /* Issued by run time error before the message */
#define S_ERROR_2      14     /* Issued by run time error after the message */
#define S_PASS_1       16     /* Issued by PASS before call */
#define S_PASS_2       18     /* Issued by PASS after call */
#define S_RESET_VIDEO  20     /* Issued after execution of ALT-O */
#define S_USE          22     /* Issued by USE */

typedef struct                /* Error information record */
{
     unsigned short err_number;
     unsigned char err_action;
     unsigned char err_class;
     unsigned char err_locus;
} ERR_INFO;

typedef struct                /* Local message information */
{
     unsigned short err;      /* Actual message number */
     char errtext[256];       /* The message text */
} MSG;

/* Comal supplied functions */

extern outchar(char c);           /* Print null terminated string */
extern runerr(unsigned char err); /* Issue Comal error */
extern report(int err,char *errtext,ERR_INFO *e,char file);
extern get_comal_msg(unsigned short err,MSG *msg);
extern STR *create_str(unsigned short length);

/* Screen cell */

typedef struct
{
     unsigned char ch;        /* Character of cell */
     unsigned char attr;      /* Attribute of cell */
} CELL;

/*----- Comal system variables (CDATA segment) ----------------------------*/

typedef struct
{
     unsigned short psp_seg;    /* Segment selector of program prefix */

     /* Variables used by the editor */

     unsigned char col;         /* Column number for editor */
     unsigned char row;         /* Row number for editor */
     unsigned char col_min;     /* Min col no. in window */
     unsigned char col_max;     /* Max col no. in window */
     unsigned char row_min;     /* Min row no. in window */
     unsigned char row_max;     /* Min row no. in window */
     unsigned char row2;
     unsigned char pagelen;     /* No. of rows on page */
     unsigned char linelen;     /* No. of chars on a line */

     unsigned char inserting;   /* 0=overwrite, 1=insert, +128=insert input */
     unsigned char text_attr;   /* Attribute used until new line */
     unsigned char err_attr;    /* Attribute of error message */
     unsigned char save_attr;   /* Save of attribute during error printing */
     unsigned char indent_attr; /* Attribute of right arrow */
     unsigned char blank_attr;  /* Attribute used for blank */
     unsigned char disp1_attr;  /* Attribute for number in display line */
     unsigned char disp2_attr;  /* Attribute for text in display line */
     unsigned char graphics;    /* 0: alpha, 1: graphics */

     unsigned char actpg;       /* Actual used display page number */

     unsigned char norm_attr;   /* Attribute of normal text (textcolor) */
     unsigned char scan_attr;

     unsigned char show_on;     /* 1: show display during editing    */
                                /* 0: don't show                     */
                                /* 2: show display always            */
                                /* 4: show top status during editing */
                                /* - never to be seen during run!    */
     unsigned char show_status; /* 00: off, 01: bottom, 10: top, 11: both */
     unsigned char show_cstat;  /* cstat last time display was generated */
     unsigned char old_shift;   /* Old storage of shift state */
     unsigned char exit_code_bye; /* Exit code to return when BYEing */
     unsigned char coprocessor;
     unsigned char fkey_error;  /* Error flagged during function key insertion */

#ifdef protect
     short collate_info;
     unsigned char collate_table[256];
#else
     unsigned char collate_info;
     char filler;               /* Not used */
     unsigned char far *collate_addr;
#endif

     unsigned char term_pos;    /* Terminating position within input field */
     unsigned char input_pos;   /* Start position within input field */
     unsigned char inp_term[512/8];  /* 512 bit terminator table */
     unsigned char term[2];     /* Termination char. for last INPUT */

     char near *par_pnt;        /* Address of parameters (psp_seg) */
     unsigned short par_len;    /* Length of parameters */
     char near *bootprog;       /* Pointer to first program to run (CDATA) */

     unsigned char curtype;     /* 0:Soft, 1:Hard, 2:Hard always on */
     unsigned char cursize;     /* 0:No size, 1-8: size */

     CELL char_inst;            /* INPUT: char lost during inserting */

     char area1[6];
     char area2[32];
     unsigned short psize;      /* Specified program segment size/load module size */
     unsigned short dsize;      /* Specified stack segment size */
     unsigned long hsize;       /* Specified heap size */
     unsigned long ssize;       /* Specified array size (combined string&dim) */
     unsigned short ksize;      /* Specified key size */
     unsigned char keyb_offs;   /* Keyboard function code offset */
     char country[2];           /* Profile variable */

     /* bios_sw[0]:
      *   xx1xxxxx       Extended keyboard interpretation
      *   xx0xxxxx       Standard keyboard interpretation
      *   xxx1xxxx       Bell on
      *   xxx0xxxx       Bell off
      *   xxxx00xx       Soft cursor
      *   xxxx01xx       Hard cursor
      *   xxxx10xx       Hard cursor always on
      *   xxxxxx00       Bios
      *   xxxxxx01       Appropriate
      *   xxxxxx10       Retrace
      *   xxxxxx11       Quick
      */
     unsigned char bios_sw[2];  /* 0=BIOS, 1=Appropriate, 2=Retrace, 3=Quick */
                                /* 0=Look for switches, 1=Don't */
} CDATA;

/*----- Comal evaluation stack segment ----------------------------------*/

typedef struct
{
     void near *etop;              /* Top of evaluation stack. (etop<efree) */
     void near *efree;             /* Max of evaluation stack */
     unsigned char numerr;         /* <>0: Numeric error number */
     unsigned char flags;          /* Flags */
     unsigned char escape;         /* CTRL-Break has been depressed */
     unsigned char cstat;          /* Mode of operation */
     unsigned short hshift;        /* Number of times to shift carry when indexing arrays */
     unsigned short hadd;          /* Value to add to get next segment in arrays (2^hshift) */
     unsigned short cdata_seg;     /* Segment of CDATA */
     unsigned short retcode[2];    /* return code from last PASS */
     char txt[256];                /* Temporary string buffer */
     void near *temp_bp;           /* Save for BP during asm.calls */
     char inbuf[256];              /* Input buffer */
     EVENT_ENTRY far *act_event;   /* Handle for actually running event */
                                   /*  =0 if no event handled at the moment */
     unsigned short act_priority;  /* Priority of handled event */
} COMAL_DATA;

extern COMAL_DATA far *cds;        /* Comal evaluation stack segment */
extern short far *csp;             /* Caller's processor stack pointer */


/*----- Standard error messages --------------------------------------------*/

#define eREPORT          0    /* REPORT error */
#define eArgument        1    /* Argument error */
#define eOverflow        2    /* Overflow */
#define eDivZero         3    /* Division by zero */
#define eSubString       4    /* Substring error */
#define eRangeErr        5    /* Value out of range */
#define eIllBound        7    /* Illegal bound */
#define eInvFormat       8    /* Invalid format string */
#define eIndexRange      10   /* Index out of range */
#define eInvFile         11   /* Invalid file name */
#define eNoMsg           26   /* No such message number local to this MODULE/STRUC */
#define eNoMsgFile       28   /* No message file for this MODULE/STRUC */
#define eEOF             201  /* End of file */
#define eAlreadyOpen     202  /* File already open */
#define eNotOpen         203  /* File not open */
#define eNotInput        204  /* Not input file */
#define eNotOutput       205  /* Not output file */
#define eTooManyFiles    209  /* Too many files open */
#define eReadErr         210  /* Read error */
#define eWriteErr        211  /* Write error */
#define eNotReady        213  /* Drive not ready */
#define ePrinter         214  /* Printer out of paper or not ready */
#define eFileNotFound    216  /* File not found */
#define eUnknownDev      217  /* Unknown device */
#define eIllOperation    218  /* Illegal operation */
#define eIOBreak         219  /* I/O break */
#define ePathNotFound    220  /* Path not found */
#define eProtected       221  /* Attempt to write on write-protected diskette */
#define eDiskFull        224  /* Disk full */
#define eAlreadyExists   225  /* File already exists */
#define eOutOfRes        226  /* Out of resources */
#define eNetErr          227  /* Network error */
#define eBusy            228  /* Busy */
#define eAccessDenied    229  /* Access denied */
