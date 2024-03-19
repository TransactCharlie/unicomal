
#include <stdio.h>
#include "comal.h"

/*
 *   This source text implements the module below
 *
 *   MODULE stack
 *     PROC push(x#)          -- Push integer
 *     FUNC pop#              -- Remove integer
 *     FUNC empty#            -- Stack empty ?
 *   ENDMODULE stack
 */

#define   SMAX 100       /* Max number of stack elements */

int  sp;                 /* Stack pointer */
long stack[SMAX];        /* Space for stack */


/*----- Print null terminated string ---------------------------------------*/

print( char *s )
{
     while( *s ) outchar( *s++ );
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
          print( "Module stack is loaded.\x0d\x0a" );
     }
     return( 0 );   /* No error */
}

/*----- Initialize handler -------------------------------------------------*
 *
 *      The function initialize() is called when the scope of this module
 *      becomes alive. It must initialize the module to its default state,
 *      so that all functions in the module can be called directly from
 *      Comal.
 */

initialize()
{
     sp = 0;        /* Empty stack */
}


/*----- Push integer -------------------------------------------------------*/

push( long x )
{
     if ( sp >= SMAX )
     {
          report( 400, "Stack is full", NULL, 0 );
     }
     else
     {
          stack[ sp++ ] = x;
     }
}

/*----- Pop integer --------------------------------------------------------*/

long pop()
{
     if ( sp == 0 )
     {
          report( 401, "Stack is empty", NULL, 0 );
     }
     else
     {
          return( stack[ --sp ] );
     }
}

/*----- Empty --------------------------------------------------------------*/

long empty()
{
     return( (long)( sp == 0 ) );
}

