
#include <stdio.h>
#include "comal.h"

/*
 *   This source text implements the module below
 *
 *   MODULE sample
 *     FUNC acker#(m#,n#)     -- Function call with parameters
 *     PROC sample1(x#,y,REF path$)  -- Handling REF parameter
 *     FUNC sample2#          -- How to return a function value
 *     FUNC strip1$(x$)       -- Comal style strings
 *     FUNC strip2$(x$)       -- C style strings
 *     PROC issue(err#)       -- Comal error handling
 *     FUNC uppercase$(x$)    -- Uppercase of character x$
 *     PROC showpositions1(REF pos#())  -- Print positions given by array
 *     PROC showpositions2(REF pos#())  -- Print positions given by array
 *     PROC show              -- Print variables in DATA and CDATA segments
 *     PROC printenv          -- Print some environment variables
 *     FUNC root(a,b,c)       -- Calculate root of equation of 2nd degree
 *   ENDMODULE sample
 */

static char values[3] = { 17, 19, 23 };

/*----- Print null terminated string ---------------------------------------*
 *
 *      Print a null terminated string on the Comal screen.
 */

void print(char far *s)
{
     if ( s != NULL )
          while ( *s )
               outchar( *s++ );
}

/*----- Print new line -----------------------------------------------------*
 *
 *      Move cursor to beginning of next line on the Comal screen.
 */

void crlf()
{
     outchar( 13 );
     outchar( 10 );
}

/*----- Print long integer -------------------------------------------------*
 *
 *      Print a long integer to the Comal screen.
 */

void print_long(long x)
{
     if ( x<0 )
     {
          outchar( '-' );
          print_long( -x );
     }
     else if ( x<10 )
     {
          outchar( (char)(x+'0') );
     }
     else
     {
          print_long( x/10 );
          outchar( (char)((x % 10)+'0') );
     }
}

/*----- Signal handler -----------------------------------------------------*
 *
 *      The signal handler is called from Comal when events of interest
 *      are occurring. This allows the module to take action to clean up
 *      when the module is discarded from memory, or to change state as
 *      Comal changes state.
 *
 *      This signal handler prints a message just after it is loaded into
 *      memory.
 *
 *      The different kinds of signals the module can receive from Comal,
 *      are listed in comal.h.
 */

short signal( short sig )
{
     /* sig is the Comal signal number (see comal.h) */

     if ( sig == S_LOAD )
     {
          outchar(12);   /* Clear screen */
          cursor(5,10);   /* At 6th line, 10th column, print: */
          print( "-+* Module sample is loaded *+-\x0d\x0a" );
     }
     return( 0 );        /* No error */
}

/*----- Initialize handler -------------------------------------------------*
 *
 *      The function initialize() is called when the scope of this module
 *      becomes alive. It must initialize the module to its default state,
 *      so that all functions in the module can be called directly from
 *      Comal.
 *
 *      In this case the function just prints a message on the screen.
 */

void initialize()
{
     /* Initialize module. Is called automatically */
     print( "Module sample is initialized.\x0d\x0a" );
}

/*----- Ackermann's function -----------------------------------------------*
 *
 *      Comal: PROC acker#(m#,n#)
 *
 *      The acker function is written using the Comal integer type long.
 */

long acker(long m, long n)
{
     if ( m==0 )
          return( n+1 );
     else if ( n==0 )
          return( acker( m-1, (long)1 ) );
     else
          return( acker( m-1, acker( m, n-1 ) ) );
}

/*----- sample1 ------------------------------------------------------------*
 *
 *      Comal: sample1(x#,y,REF path$)
 *
 *      This function demonstrates three of Comal's types mapped
 *      into C-types:
 *        x#        -> long x      -- long is the same as Comal's integer type.
 *        y         -> double y    -- double matches Comal's floating point type.
 *        REF path$ -> char far *path  -- Comal style string is mapped into
 *                                        a null-terminated string. When this
 *                                        function returns to Comal, it is
 *                                        mapped back to a Comal-style string.
 *
 *      This function assigns  path$:="test".
 */

void sample1(long x, double y, char far *path)

{
     /* path$:="test" */

     strcpy(path, "test");
}

/*----- sample2 ------------------------------------------------------------*
 *
 *      Comal: FUNC sample2#
 *
 *      The type of the result is char. It is converted to Comal's
 *      integer type when returning back to Comal.
 *
 *      This function just returns an integer.
 */

char sample2()
{
    return( values[1] ); /* Return 19 */
}


/*----- Strip 1 ------------------------------------------------------------*
 *
 *      Comal: FUNC strip1$(x$)
 *
 *      Strip a string for spaces using Comal style strings.
 */


STR *strip1(STR *x)
{
    unsigned short i, j;

    /* Do the work in the parameter string itself */
    for( i = j = 0; i < x->cur_length; i++)
    {
        if ( x->string[i] != ' ' )
        {
            x->string[j++] = x->string[i];
        }
    }
    x->cur_length = j;
    return( x );
}

/*----- Strip 2 ------------------------------------------------------------*
 *
 *      Comal: FUNC strip2$(x$)
 *
 *      Strip a string for spaces using null terminated strings.
 */

char far *strip2(char far *x)
{
    char far *src = x;  /* Source */
    char far *dst = x;  /* destination */

    /* Do the work in the parameter string itself */

    while ( *src )
    {
        if ( *src != ' ' )
        {
            *dst++ = *src;
        }
        src++;
    }
    *dst = 0;

    return( x );
}

/*----- Issue error --------------------------------------------------------*
 *
 *      Comal: PROC issue(err#)
 *
 *      When it is necessary to issue error messages from within a C-module,
 *      it can be done with the function runerr. This function corresponds
 *      to the Comal statement:  REPORT err#
 *      It is treated as any other Comal error, i.e. it can caught in the
 *      TRAP-HANDLER-ENDTRAP program structure.
 *
 *      The runerr function can only issue the standard messages in the
 *      range from 0 to 255.
 *
 *      When runerr is called, control is transferred to the Comal system.
 *      The C-module remains in the state it was when runerr was called.
 */

void issue(unsigned char err)
{
     /* Issue error inside Comal */
     runerr( err );
     /* Try also: report( err, "Error given by 'issue'", NULL, 0 ); */
}

/*----- Error --------------------------------------------------------------*
 *
 *      Comal: PROC error(err#)
 *
 *      When the module has a message file, then this function should be used.
 *      If standard messages are included in the samplei.asm file, and no
 *      message file was found, the standard messages are used.
 *
 *      The standard messages for this module are:
 *
 *        0: 'Sample module - version 1.0'
 *        1: 'Sample module - error 1'
 *        2: 'Sample module - error 2'
 *        3: 'Sample module - error 3'
 *        4: 'Sample module - error 4'
 *
 *      The standard messages can also be accessed from Comal.
 *      Example:
 *        FOR errno#:=0 TO 4 DO PRINT ERRTEXT$(sample.ERR(errno#))
 *
 */

void error(short err)
{
     MSG msg;                      /* Allocate space for message */
     get_comal_msg( err, &msg );   /* Translate err and get message */
     report( msg.err, msg.errtext, NULL, 0 ); /* Let Comal handle the error */
}

/*----- Upper case ---------------------------------------------------------*
 *
 *      Comal: FUNC uppercase$(x$)
 *
 *      A Comal string can be mapped to the C type char, and the function
 *      result of type char can be converted back to Comal's string type.
 *
 *      This function returns returns the specified character as an upper case
 *      letter. Only the letters from a-z are folded.
 */

char uppercase(char x)
{
     if ( ( x >= 'a' ) && ( x <= 'z' ) )
          return( x-'a'+'A' );
     else
          return( x );
}

/*----- Showpositions1 -----------------------------------------------------*
 *
 *      Comal: PROC showpositions1(REF pos#())
 *
 *      An example using Comal style arrays. A pointer to an array descriptor
 *      is passed to showposition1. The descriptor contains the following
 *      fields:
 *        base         -- Points to an area of contiguous array components.
 *        idx[0]       -- Describes the index space.
 *        idx[0].ilen  -- Number of elements in vector.
 *
 *      This function prints a line where the column positions given
 *      by the array are printed as digits and all other positions as '-'.
 */

void showpositions1(INT_ARRAY *pos)
{
     unsigned int i;         /* Loop counter */
     int cur = 0;            /* Current position */
     long far *base;         /* Array base */

     base = pos->base;

     for( i=0; i<pos->idx[0].ilen; i++)
     {
          for(; cur<base[i]; cur++)
               outchar( '-' );
          outchar( (char)((cur % 10) + '0') );
          cur++;
     }
     crlf();
}

/*----- Showpositions2 -----------------------------------------------------*
 *
 *      Comal: PROC showpositions2(REF pos#())
 *
 *      A different version of showpositions1. This function is passed the
 *      Comal array as two C-parameters:
 *        pos          -- Points to an area of contiguous array components.
 *        length       -- Number of elements in vector.
 *
 *      This function prints a line where the column positions given
 *      by the array are printed as digits and all other positions as '-'.
 */

void showpositions2(long far *pos, unsigned short length)
{
     unsigned int i;         /* Loop counter */
     int cur = 0;            /* Current position */

     for( i=0; i<length; i++)
     {
          for(; cur<pos[i]; cur++)
               outchar( '-' );
          outchar( (char)((cur % 10) + '0') );
          cur++;
     }
     crlf();
}

/*----- Show ---------------------------------------------------------------*
 *
 *      Show variables from Comal's data segments.
 *
 *      This function demonstrates how to access system variables
 *      within the Comal system.
 */

typedef struct
{
     unsigned short offset;        /* Pointer offset */
     unsigned short segment;       /* Pointer segment selector */
} farptr;

typedef union
{
     farptr rec;                   /* Either offset & segment, */
     CDATA far *ptr;               /* or a far pointer.        */
} CDATA_SEG;

show()
{
     CDATA_SEG cdata;         /* Points to Comal segment CDATA, see comal.h */

     print( "Variables inside Comal." ); crlf();
     crlf();
     print( "etop        : " ); print_long( (long)(unsigned short)cds->etop ); crlf();
     print( "efree       : " ); print_long( (long)(unsigned short)cds->efree  ); crlf();
     print( "numerr      : " ); print_long( (long)cds->numerr ); crlf();
     print( "flags       : " ); print_long( (long)cds->flags  ); crlf();
     print( "escape      : " ); print_long( (long)cds->escape ); crlf();
     print( "hshift      : " ); print_long( (long)cds->hshift ); crlf();
     print( "hadd        : " ); print_long( (long)cds->hadd   ); crlf();
     print( "cdata_seg   : " ); print_long( (long)cds->cdata_seg ); crlf();

     /* Let cdata point to CDATA segment */
     cdata.rec.offset  = 0;
     cdata.rec.segment = cds->cdata_seg;

     crlf();
     print( "Variables in CDATA." ); crlf();
     crlf();
     print( "psp_seg     : " ); print_long( (long)cdata.ptr->psp_seg ); crlf();
     print( "col_min     : " ); print_long( (long)cdata.ptr->col_min ); crlf();
     print( "cal_max     : " ); print_long( (long)cdata.ptr->col_max ); crlf();
     print( "row_min     : " ); print_long( (long)cdata.ptr->row_min ); crlf();
     print( "row_max     : " ); print_long( (long)cdata.ptr->row_max ); crlf();
     print( "term_pos    : " ); print_long( (long)cdata.ptr->term_pos ); crlf();
     print( "cursize     : " ); print_long( (long)cdata.ptr->cursize ); crlf();
     print( "country     : " ); outchar( cdata.ptr->country[0] );
                                outchar( cdata.ptr->country[1] ); crlf();
}

/*----- printenv -----------------------------------------------------------*
 *
 *      Prints some environment variables.
 *
 *      The function getenv searches Comal's environment space.
 *      The environment variable can be placed in the profile file.
 */

extern char far *getenv(char *varname);

void printenv()
{
     print( "Some environment variables ..." ); crlf();
     print( "  PATH=" );     print( getenv( "PATH" ) );     crlf();
     print( "  COMSPEC=" );  print( getenv( "COMSPEC" ) );  crlf();
     print( "  PROMPT=" );   print( getenv( "PROMPT" ) );   crlf();
     print( "  NOTFOUND=" ); print( getenv( "NOTFOUND" ) ); crlf();
}

/*----- root ---------------------------------------------------------------*
 *
 *      Calculate a root of an equation of 2nd degree.
 *
 */

double root(double a, double b, double c)
{
#if 0
    return 1.2;
#else
    return (-b+sqrt(b*b-4*a*c))/(2*a);
#endif
}
