
;
;  This file contains a sample C-interface.
;
INCLUDE modc.inc
;
;
;-------- Interface between Comal and C-module -------------------------------
;
CMODEL                  ;Use /Dmodel to select storage model
;
;-------- Standard messages to use when there is no message file
;
CSTDMSG 4               ;The number of the last message is specified
  CMSG  0, 'Sample module - version 1.1'
  CMSG  1, 'Sample module - error 1'
  CMSG  2, 'Sample module - error 2'
  CMSG  3, 'Sample module - error 3'
  CMSG  4, 'Sample module - error 4'
CENDSTDMSG
;
;       C-functions
;       ===========
;
CEXTRN  signal          ;short signal(short sig)
CEXTRN  initialize      ;initialize()
CEXTRN  sample1         ;sample1(long x, double y, char far *path)
CEXTRN  sample2         ;char sample2()
CEXTRN  acker           ;long acker(long m, long n)
CEXTRN  strip1          ;STR *strip1(STR *x)
CEXTRN  strip2          ;char far *strip2(char far *x)
CEXTRN  issue           ;issue(unsigned char err)
CEXTRN  error           ;error(short err)
CEXTRN  uppercase       ;char uppercase(char x)
CEXTRN  showpositions1  ;showpositions1(INT_ARRAY *pos)
CEXTRN  showpositions2  ;showpositions2(INT far *pos,unsigned short length)
CEXTRN  show            ;show()
CEXTRN  printenv        ;printenv()
CEXTRN  root            ;double root(double a, double b, double c)
;
;
;       Module interface code
;       =====================
;
CMODULE sample          ;Module interface begin
;
;------ Macros for pushing parameters, calling, and returning
;
;       Beginning:
;         CPROC name                -- Begin procedure
;         CFUNCI name               -- Begin integer function
;         CFUNCR name               -- Begin real function
;         CFUNCS name               -- Begin string function
;         CBEGIN name               -- Begin procedure/function
;
;       Preprocessing:
;         real_to_double            -- r           -> double r
;         real_to_double_ptr        -- r           -> double far *r
;         ref_real_to_double_ptr    -- REF r       -> double far *r
;
;         int_to_char               -- i#          -> [unsigned ]char i
;         ref_int_to_char_ptr       -- REF i#      -> [unsigned ]char far *i
;         int_to_short              -- i#          -> [unsigned ]short i
;         ref_int_to_short_ptr      -- REF i#      -> [unsigned ]short far *i
;         int_to_long               -- i#          -> [unsigned ]long i
;         ref_int_to_long_ptr       -- REF i#      -> [unsigned ]long far *i
;
;         str_to_char               -- s$          -> [unsigned ]char s
;         ref_str_to_char           -- REF s$      -> [unsigned ]char s
;         str_to_char_ptr           -- s$          -> char far *s
;         ref_str_to_char_ptr       -- REF s$      -> char far *s
;         str_to_str                -- s$          -> STR *s
;         ref_str_to_str            -- REF s$      -> STR *s
;
;         ptr_to_ptr                -- p@ OF t     -> t far *p
;                                   -- REF q. OF t -> t far *q
;         ref_ptr_to_ptr_ptr        -- REF p@ OF t -> t far **p
;
;         array_to_ptr              -- a()         -> type_ARRAY *a
;         ref_array_to_ptr          -- REF a()     -> type_ARRAY *a
;         vec_to_vec_len            -- a()         -> t far *a, unsigned short length
;         ref_vec_to_vec_len        -- REF a()     -> t far *a, unsigned short length
;
;       Calling:
;         CALL_void                 -- p()                   -> PROC p
;         CALL_char_to_int          -- char f()              -> FUNC f#
;         CALL_uchar_to_int         -- unsigned char f()     -> FUNC f#
;         CALL_short_to_int         -- short f()             -> FUNC f#
;         CALL_ushort_to_int        -- unsigned short f()    -> FUNC f#
;         CALL_long_to_int          -- long f()              -> FUNC f#
;         CALL_double_to_real       -- double f()            -> FUNC f
;         CALL_char_to_str          -- char f()              -> FUNC f$
;         CALL_char_ptr_to_str      -- char far *f()         -> FUNC f$
;         CALL_str_to_str           -- STR *f()              -> FUNC f$
;         CALL_ptr_to_int           -- t far *f()            -> FUNC f#
;
;       Postprocessing:
;         char_ptr_to_ref_int       -- char far *i           -> REF i#
;         uchar_ptr_to_ref_int      -- unsigned char far *i  -> REF i#
;         short_ptr_to_ref_int      -- short far *i          -> REF i#
;         ushort_ptr_to_ref_int     -- unsigned short far *i -> REF i#
;         char_ptr_to_ref_str       -- char far *s           -> REF s$
;
;------------------------------------------------------------------------------
;
;       Signal handler for this module
;       ==============================
;
;       C:      int signal(int type)
;
CSIGNAL signal
;
;
;       Initialisation handler for sample
;       ================================
;
;       C:      initialize()
;
CINIT   initialize
;
;
;====== Implementation of procedure & function bodies ========================
;
;
;       Comal: PROC sample1(x#,y,REF path$)
;       C:     sample1(long x, double y, char far *path)
;
;       Parameters are pushed in reverse order to sample1.
;
;       Preprocessing:
;         REF path$ is converted into ASCIIZ string:  ref_str_to_char_ptr
;         y         is compatible with 'double y'  :  real_to_double
;         x#        is compatible with 'long x'    :  int_to_long
;
;       Call:
;         It is a void function                    :  CALL_void sample1
;
;       Postprocessing:
;         REF path$ is converted from char *       :  char_ptr_to_ref_str path
;
CPROC   sample1
           ;Push in reverse order
        ref_str_to_char_ptr     ;REF path$ -> char *
        real_to_double          ;y         -> double y
        int_to_long             ;x#        -> long x
           ;Call & return
        CALL_void sample1       ;sample1(x#,y,path$)
           ;Postprocessing
        char_ptr_to_ref_str path
CEND
;
;
;       Comal: FUNC sample2#
;       C:     char sample2()
;       ====================
;
CFUNCI  sample2
        CALL_char_to_int        ;sample2()
CEND
;
;
;       Comal:  FUNC acker#(m#,n#)
;       C:      long acker(long m, long n)
;       ==================================
;
CFUNCI  acker
        int_to_long             ;n#     -> long n
        int_to_long             ;m#     -> long m
        CALL_long_to_int        ;acker(n#,m#)
CEND
;
;
;       Comal:  FUNC strip1$(x$)
;       C:      STR *strip1(STR *x)
;       ===========================
;
;       The strip1 function uses Comal style strings.
;
CFUNCS  strip1
        str_to_str              ;x$     -> STR *x
        CALL_str_to_str         ;strip1(x)
CEND
;
;
;       Comal:  FUNC strip2$(x$)
;       C:      char far *strip2(char far *x)
;       =====================================
;
;       Function strip2 uses null terminated strings.
;
CFUNCS  strip2
        str_to_char_ptr         ;x$     -> char far *x
        CALL_char_ptr_to_str    ;strip2(x)
CEND
;
;
;       Comal:  PROC issue(err#)
;       C:      issue(unsigned char err)
;       ================================
;
CPROC   issue
        int_to_char             ;err#   -> unsigned char err
        CALL_void               ;issue(err)
CEND
;
;
;       Comal:  PROC error(err#)
;       C:      error(short err)
;       ========================
;
CPROC   error
        int_to_short            ;err#   -> short err
        CALL_void               ;error(err)
CEND
;
;
;       Comal:  FUNC uppercase$(x$)
;       C:      char uppercase(char x)
;       ==============================
;
CFUNCS  uppercase
        str_to_char             ;x$     -> char x
        CALL_char_to_str
CEND
;
;
;       Comal:  PROC showpositions1(REF pos#())
;       C:      showpositions1(INT_ARRAY *pos)
;       ======================================
;
CPROC   showpositions1
        ref_array_to_ptr        ;REF pos#() -> INT_ARRAY *pos
        CALL_void
CEND
;
;
;       Comal:  PROC showpositions2(REF pos#())
;       C:      showpositions1(INT far *pos, unsigned short length)
;       =========================================================
;
CPROC   showpositions2
        ref_vec_to_vec_len          ;REF pos#() -> INT far *pos, unsigned short length
        CALL_void
CEND
;
;
;       Comal:  PROC show
;       C:      show()
;       =================
;
CPROC   show
        CALL_void
CEND
;
;
;       Comal:  PROC printenv
;       C:      printenv()
;       ==================
;
CPROC   printenv
        CALL_void
CEND
;
;
;       Comal:  FUNC root(a,b,c)
;       C:      double root(double a, double b, double c)
;
CFUNCR  root
        real_to_double              ;c -> double c
        real_to_double              ;b -> double b
        real_to_double              ;a -> double a
        CALL_double_to_real
CEND
;
CENDMODULE                      ;Module interface end
