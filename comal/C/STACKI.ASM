;
;------ C-Interface code for module stack
;
INCLUDE modc.inc
;
CMODEL                          ;Use /Dmodel to select storage model
;
;       C-functions
;       ===========
;
CEXTRN  signal                  ;short signal(short code)
CEXTRN  initialize              ;initialize()
CEXTRN  push                    ;push(long x)
CEXTRN  pop                     ;long pop()
CEXTRN  empty                   ;long empty()

CMODULE stack

  CSIGNAL signal                ;signal(short sig)
  CINIT   initialize            ;initialize()

  CPROC   push                  ;PROC push(x#) -> push(long x)
    int_to_long                 ;x#            -> long x
    CALL_void                   ;call
  CEND

  CFUNCI  pop                   ;FUNC pop#     -> long pop()
    CALL_long_to_int
  CEND

  CFUNCI  empty                 ;FUNC empty#   -> long empty()
    CALL_long_to_int
  CEND

CENDMODULE
