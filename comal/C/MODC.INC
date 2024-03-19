.xlist
PAGE    66,132
;----------------------------------------------------------------------------+
;                                                                            |
;   U    U             CCCC                          l      (C) Copyright    |
;   U    U            C                              l       UniComal A/S    |
;   U    U  nnnn   i  C       ooo   mmmmmm    aaaa   l          1990         |
;   U    U  n   n  i  C      o   o  m  m  m  a   a   l                       |
;   U    U  n   n  i  C      o   o  m  m  m  a   a   l        80x86 COMAL    |
;    UUUU   n   n  i   CCCC   ooo   m  m  m   aaaaa   ll    ===============  |
;                                                                            |
;----------------------------------------------------------------------------+
;
;       File: MODC.INC
;       Symbols for use by C modules for COMAL.
;
;       This file is included in the interface code files
;       for C modules for DOS or OS/2 Comal.
;
;       UniComal Version 3.00/3.01/3.02/3.03/3.10x/3.11
;
;-----------------------------------------------------------------------------
;
;------ Routines supplied by COMAL:
;
EXTRN   runerr:FAR
EXTRN   filerr:FAR
EXTRN   report:FAR
EXTRN   get_err_msg:FAR
EXTRN   get_err_num:FAR
EXTRN   get_file:FAR
EXTRN   set_file:FAR
EXTRN   fpadd:FAR
EXTRN   fpsub:FAR
EXTRN   fpsubr:FAR
EXTRN   fpmul:FAR
EXTRN   fpdiv:FAR
EXTRN   fpdivr:FAR
EXTRN   fpint:FAR
EXTRN   intfp:FAR
EXTRN   fpsqr:FAR
EXTRN   fpsin:FAR
EXTRN   fpcos:FAR
EXTRN   fptgn:FAR
EXTRN   fpatn:FAR
EXTRN   fplog:FAR
EXTRN   fpexp:FAR
EXTRN   outchar:FAR
EXTRN   bye:FAR
EXTRN   create_str:FAR
EXTRN   remove_str:FAR
EXTRN   dir_entry:FAR
EXTRN   check_dir:FAR
EXTRN   get_char:FAR
EXTRN   put_char:FAR
EXTRN   editor_functions:FAR
EXTRN   move_from:FAR
EXTRN   move_to:FAR
EXTRN   link_event:FAR
EXTRN   scan_env:FAR
EXTRN   set_test_key:FAR
EXTRN   set_read_key:FAR
;
EXTRN   ml_prog:WORD            ;Start of PROG segment
perr_seg EQU    WORD PTR 1AH    ;Segment of messages
perr_ofs EQU    WORD PTR 1CH    ;Error number offset
perr_max EQU    WORD PTR 1EH    ;Last error number + 1
;
.LALL                           ;Suppress macro listing
;
IFDEF   protect                 ;If protect mode, allow 286 intructions
         .286C
ENDIF
;
intvar  STRUC
loww      DW ?                  ;Low part of integer
highw     DW ?                  ;High part of integer
intvar  ENDS
;
farptr  STRUC
offw      DW ?                  ;Offset of far pointer
segw      DW ?                  ;Segment selector of far pointer
farptr  ENDS
;
;
;------ String variable layout:
;
strvar  STRUC
maxlen    DW ?                  ;Dimensioned length
curlen    DW ?                  ;Current length of string
s         DB 1 DUP(?)           ;Storage for string
strvar  ENDS
;
;
;------ Array dope vector layout:
;
adope   STRUC                   ;Dope vector for arrays
abase     DD    ?               ;Address of first array component
awaste    DW    ?               ;No. of bytes wasted before array - for alignment!
aibase    DW    ?               ;Element base in first segment (used in OS/2 only)
aisize    DW    ?               ;Elements per segment (used in OS/2 only)
asize     DW    ?               ;Size of each element (always even)
adim4     DW    ?               ;Dimension*4
alow1     DW    ?               ;First lowbound(negated) (-32767..32767)
alen1     DW    ?               ;Length of index space (0..65535)
alow2     DW    ?               ;Second lowbound(negated) (-32767..32767)
alen2     DW    ?               ;Length of index space (0..65535)
alow3     DW    ?               ;Third lowbound(negated) (-32767..32767)
alen3     DW    ?               ;Length of index space (0..65535)
                                ; etc...
adope   ENDS
;
;
;------ Index space:
;
index   STRUC                   ;Index space record
ilow      DW    ?               ;Lowbound(negated) (-32767..32767)
ilen      DW    ?               ;Length of index space (0..65535)
index   ENDS
;
;
;------ Event message buffer:
;
event_entry STRUC               ;Event entry record
next_event    DD ?              ;Link to next event
priority      DW ?              ;Priority of this event
proc_handle   DQ ?              ;Descriptor of PROCedure to execute
event_entry ENDS
;
;
;  Open file modes:
;
o_read  EQU     01H             ;Opened for reading
o_write EQU     02H             ;Opened for writing
;
l_read  EQU     04H             ;Deny read
l_write EQU     08H             ;Deny write
;
o_ran   EQU     10H             ;Opened for random access
;
x_commit EQU    20H             ;Commit each WRITE/PRINT
x_app   EQU     40H             ;Append to file
x_locks EQU     80H             ;Explicit LOCKs is defined
;
trap_esc EQU    4               ;flags: TRAP ESC- flag
breaked  EQU    8               ;flags: ESC value
;
;compiler EQU   10B              ;system: 0=Interactive, 1=Compiled
;scr_sw  EQU  1100B              ;system: 00=/B, 01=/A, 10=/R, 11=/Q
;cur_sw  EQU 10000B              ;system: 0=/S, 1=/H
;
;
;------ ML signal numbering
;
s_load          EQU     0       ;Issued during CHAIN, RUN, SCAN
s_discard       EQU     2       ;Issued by NEW, LOAD, CHAIN, BYE(, RUN, SCAN)
s_link          EQU     4       ;Issued during RUN, CHAIN
s_unlink        EQU     6       ;Issued before SCAN, RUN, CHAIN, NEW, BYE
s_stop          EQU     8       ;Issued by STOP, END or CTRL-Break
s_con           EQU    10       ;Issued by CON
s_error_1       EQU    12       ;Issued by run time error before the message
s_error_2       EQU    14       ;Issued by run time error after the message
s_pass_1        EQU    16       ;Issued by PASS before call
s_pass_2        EQU    18       ;Issued by PASS after call
s_reset_video   EQU    20       ;Issued after execution of ALT-O
s_use           EQU    22       ;Issued in USE-statement
;
;
;------ COMAL data segment.
;
data    SEGMENT AT 0
ASSUME  CS:data
etop            DW ?    ;Top of evaluation stack. (etop<efree)
efree           DW ?    ;Max of evaluation stack
numerr          DB ?    ;<>0: Numeric error number
flags           DB ?    ;Flags
escape          DB ?    ;CTRL-Break has been depressed
cstat           DB ?    ;Mode of operation
hshift          DW ?    ;Number of times to shift carry when indexing arrays
hadd            DW ?    ;Value to add to get next segment in arrays (2^hshift)
;
cdata_seg       DW ?    ;Segment of cdata
;
retcode         DD ?                    ;ret. code from last PASS
txt             DB 100h DUP(?)          ;Translated string constant.
temp_bp         DW ?                    ;Save for BP during asm.calls
inbuf           DB 100h DUP(?)          ;Input buffer.
;
act_event       DD ?                    ;Handle for actually running event
                                        ;=0 if no event handled at the moment
act_priority    DW ?                    ;Priority of handled event
data    ENDS
;
;------ COMAL system variables.
;
cdata_s SEGMENT AT 0
;
psp_seg  DW ?                   ;Segment of program prefix
;
; Variables used by the editor
;
col     DB      ?               ;column number for editor
row     DB      ?               ;row number for editor
col_min DB      ?               ;min col no. if in window. must start on word boundary.
col_max DB      ?               ;max col no. if in window
row_min DB      ?               ;min row no. if in window
row_max DB      ?               ;min row no. if in window
row2    DB      ?
pagelen DB      ?               ;no of rows on page.
linelen DB      ?               ;no of chars on a line.
;
inserting DB    ?               ;0=replace, 1=insert, +128=insert input
text_attr DB    ?               ;attribute of text displayed
err_attr DB     ?               ;attribute of error message
save_attr DB    ?               ;save of attribute during error printing
indent_attr DB  ?               ;attribute of right arrow
blank_attr DB   ?               ;attribute used for blank
disp1_attr DB   ?               ;attribute for number in display line
disp2_attr DB   ?               ;attribute for text in display line
graphics DB     ?               ;0: alpha, 1: graphics
;
actpg       DB  ?               ;actual page number.
;
norm_attr   DB  ?               ;Attribute of normal text
;
scan_attr   DB  0
;
show_on         DB      5       ;1: show display during editing
                                ;0: don't show
                                ;2: show display always.
                                ;4: show top status during editing.
                                ;- never to be seen during run!
show_status     DB      ?       ;00: off, 01: bottom, 10: top, 11: both
show_cstat      DB      0       ;cstat last time display was generated
;
old_shift       DB      ?       ;old storage of shift state
;
exit_code_bye DB 0              ;Code to return when BYEing
;
coprocessor DB 0
;
fkey_error  DB  ?               ;Error flagged during function key insertion
;
IFDEF   protect
        EVEN
collate_info DW 0
collate_table DB 256 DUP(?)
;
ELSE
collate_info DB ?
        EVEN
collate_addr DD 0
;
ENDIF
;
term_pos    DB  0               ;Terminating position within input field
input_pos   DB  0               ;Start position within input field
inp_term        DB  512/8  DUP(?)       ;512 bit terminator table
term            DW      ?               ;Termination char. for last INPUT
;
par_pnt  DW 0                   ;Address of parameters
par_len  DW 0                   ;Length of parameters
bootprog DW 0                   ;Pointer to first program to run
;
curtype     DB  0               ;0:Soft, 1:Hard, 2:Hard always on
cursize     DB  2               ;0:No size, 1-8: size
;
char_inst   DW 0                ;char lost during inserting
;
;------ Compiler variables ------------------------------------------------
;       The order and amount is used by COMALC.EXE
;
        EVEN
        DB      6 DUP(?)
        DB  32 DUP(0)
psize   DW 0FFFEH       ;Specified program segment size/load module size
dsize   DW 0FFFEH       ;Specified stack segment size
hsize   DD 0FFFEH       ;Specified heap size
ssize   DD 0FFFEH       ;Specified array size (combined string&dim)
ksize   DW 00800H       ;Specified key size
keyb_offs DB 0          ;Keyboard function code offset
                        ;-1: do Test_keyboard
country DB      'US'    ;COUNTRY="US" // profile variable
;
;       bios_sw:
;         xx1xxxxx      ;Extended keyboard interpretation
;         xx0xxxxx      ;Standard keyboard interpretation
;         xxx1xxxx      ;Bell on
;         xxx0xxxx      ;Bell off
;         xxxx00xx      ;Soft cursor
;         xxxx01xx      ;Hard cursor
;         xxxx10xx      ;Hard cursor always on
;         xxxxxx00      ;Bios
;         xxxxxx01      ;Appropriate
;         xxxxxx10      ;Retrace
;         xxxxxx11      ;Quick
;
IFDEF   protect
bios_sw  DB 10000B      ;0=BIOS, 1=Appropriate, 2=Retrace, 3=Quick
ELSE
bios_sw  DB 10001B      ;0=BIOS, 1=Appropriate, 3=Quick
ENDIF
;ENDIF
         DB 0           ;0=Look for switches, 1=Don't
;
msg1    DB 500 DUP(?)
msg1_len DW     ?
regno   DD      ?
userno  DD      ?
;
;
;------ End of compiler variables -----------------------------------------
;
cdata_s ENDS
;
;
;------ Macros to get addresses of small parameters (integers/reals)
;
@val_smal MACRO ofs,nam
        LEA     ofs,[DI+nam]
       ENDM
;
@ref_smal MACRO sgm,ofs,nam
        L&sgm   ofs,[DI+nam]
       ENDM
;
; Macros to get addresses of huge parameters (strings/strucs/arrays)
;
@val_huge MACRO sgm,ofs,nam
        L&sgm   ofs,[DI+nam]
       ENDM
;
@ref_huge MACRO sgm,ofs,nam
        L&sgm   ofs,[DI+nam]
       ENDM
;
;------ Macros for pushing parameters, calling, and returning
;
;       Beginning:
;         CPROC name                -- Begin procedure
;         CFUNCI name               -- Begin integer function
;         CFUNCR name               -- Begin real function
;         CFUNCS name               -- Begin string function
;         CBEGIN cname, name        -- Begin procedure/function
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

pstr1=0   ;Assume pchar_to_str not used
pstr2=0   ;Assume pstr_to_str not used

;------ Begin procedure

CPROC   MACRO function
        CBEGIN function, P_&function
;
;------ Define movref macro
;
movref  &MACRO segm, reg, offset
        L&&segm reg,[DI+coffset].&function&_&&offset
        &ENDM
        ENDM

;------ Begin integer function

CFUNCI  MACRO function
        CBEGIN function, F_&function&I
;
;------ Define movref macro
;
movref  &MACRO segm, reg, offset
        L&&segm reg,[DI+coffset].&function&I_&&offset
        &ENDM
        ENDM

;------ Begin real function

CFUNCR  MACRO function
        CBEGIN function, F_&function&R
;
;------ Define movref macro
;
movref  &MACRO segm, reg, offset
        L&&segm reg,[DI+coffset].&function&R_&&offset
        &ENDM
        ENDM

;------ Begin string function

CFUNCS  MACRO function
        CBEGIN function, F_&function&S
;
;------ Define movref macro
;
movref  &MACRO segm, reg, offset
        L&&segm reg,[DI+coffset].&function&S_&&offset
        &ENDM
        ENDM

;------ General beginning

CBEGIN  MACRO cname, function
CEND    &MACRO
        &RET
function &ENDP
        &ENDM
;
;------ Define ccall macro
;
ccall   &MACRO name
IFNB    <name>
        CALL    _&&name         ;Call C-function
ELSE
        CALL    _&cname         ;Call C-function
ENDIF
        &ENDM
;
        PUBLIC function
function PROC FAR
;Change to C-module's stack
        MOV     CX,SS           ;Save COMAL stack until later
        MOV     DX,DGROUP
        MOV     BX,SP
        MOV     SS,DX
ASSUME  SS:DGROUP
        MOV     SP,SS:[__atopsp];Start of stack
        MOV     [_csp].offw,BX
coffset=0                       ;Points after last parameter
        ENDM

;------ r -> double r

real_to_double MACRO
coffset=coffset-8
        PUSH    WORD PTR [DI+6].coffset
        PUSH    WORD PTR [DI+4].coffset
        PUSH    WORD PTR [DI+2].coffset
        PUSH    WORD PTR [DI+0].coffset
        ENDM

;------ r -> double far *r

real_to_double_ptr MACRO
coffset=coffset-8
        LEA     AX,[DI].coffset
        PUSH    DS
        PUSH    AX
        ENDM

;------ REF r -> double far *r

ref_real_to_double_ptr MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ i# -> char i

int_to_char MACRO
coffset=coffset-4
        PUSH    [DI].coffset.loww
        ENDM

;------ REF i# -> char far *i

ref_int_to_char_ptr MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ i# -> short i

int_to_short MACRO
coffset=coffset-4
        PUSH    [DI].coffset.loww
        ENDM

;------ REF i# -> short far *i

ref_int_to_short_ptr MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ i# -> long i

int_to_long MACRO
coffset=coffset-4
        PUSH    [DI].coffset.highw
        PUSH    [DI].coffset.loww
        ENDM

;------ REF i# -> long far *i

ref_int_to_long_ptr MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ s$ -> char s

str_to_char MACRO
coffset=coffset-4
        LES     SI,[DI].coffset
        MOV     AL,ES:[SI].s[0]
        CBW
        PUSH    AX
        ENDM

;------ REF s$ -> char s (faster when variable)

ref_str_to_char MACRO
coffset=coffset-4
        LES     SI,[DI].coffset
        MOV     AL,ES:[SI].s[0]
        CBW
        PUSH    AX
        ENDM

;------ s$ -> char far *s

str_to_char_ptr MACRO
coffset=coffset-4
        LES     BX,[DI].coffset
        MOV     SI,ES:[BX].curlen
        ADD     BX,s
        MOV     BYTE PTR ES:[BX+SI],0       ;Zero terminate
        PUSH    ES
        PUSH    BX
        ENDM

;------ REF s$ -> char far *s

ref_str_to_char_ptr MACRO
coffset=coffset-4
        LES     BX,[DI].coffset
        MOV     SI,ES:[BX].curlen
        ADD     BX,s
        MOV     BYTE PTR ES:[BX+SI],0       ;Zero terminate
        PUSH    ES
        PUSH    BX
        ENDM

;------ s$ -> STR *s

str_to_str MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ REF s$ -> STR *s

ref_str_to_str MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ p@ OF t -> t far *p

ptr_to_ptr MACRO
coffset=coffset-4
        PUSH    [DI].coffset.segw
        PUSH    [DI].coffset.offw
        ENDM

;------ REF p@ OF t -> t far **p

ref_ptr_to_ptr_ptr MACRO
coffset=coffset-4
        PUSH    [DI].coffset.highw
        PUSH    [DI].coffset.loww
        ENDM

;------ a() -> type_ARRAY *a

array_to_ptr MACRO
        ptr_to_ptr
        ENDM

;------ REF a() -> type_ARRAY *a

ref_array_to_ptr MACRO
        ptr_to_ptr
        ENDM

;------ a() -> t far *a, unsigned short length

vec_to_vec_len MACRO
coffset=coffset-4
        LES     BX,[DI].coffset
        PUSH    ES:[BX].alen1           ;unsigned short length
        PUSH    ES:[BX].abase.segw      ;t *a[]
        PUSH    ES:[BX].abase.offw
        ENDM

;------ REF a() -> t far *a, unsigned short length

ref_vec_to_vec_len MACRO
        vec_to_vec_len
        ENDM

;------ Call C-function, return nothing

CALL_void MACRO function
        MOV     DS,DX                   ;Now SS and DS points to DGROUP
ASSUME  DS:DGROUP
        MOV     [_csp].segw,CX          ;Save COMAL stack segment
;
        ccall   function                ;Call C-function
;
        MOV     ES,[_cds].segw
ASSUME  ES:data
;;      MOV     DI,[etop]               ;Reload evaluation stack pointer
        MOV     SS,[_csp].segw          ;Reload COMAL stack segment
ASSUME  SS:NOTHING
        MOV     SP,[_csp].offw
;
        PUSH    ES                      ;DS and ES is data
        POP     DS
ASSUME  DS:data
coffset=0
        ENDM

;------ Call C-function, return char as integer

CALL_char_to_int MACRO function
        CALL_void function
        CBW
        CWD
        STOSW
        XCHG    AX,DX
        STOSW
coffset=-4
        ENDM

;------ Call C-function, return unsigned char as integer

CALL_uchar_to_int MACRO function
        CALL_void function
        SUB     AH,AH
        STOSW
        SUB     AX,AX
        STOSW
coffset=-4
        ENDM

;------ Call C-function, return short as integer

CALL_short_to_int MACRO function
        CALL_void function
        CWD
        STOSW
        XCHG    AX,DX
        STOSW
coffset=-4
        ENDM

;------ Call C-function, return unsigned short as integer

CALL_ushort_to_int MACRO function
        CALL_void function
        STOSW
        SUB     AX,AX
        STOSW
coffset=-4
        ENDM

;------ Call C-function, return long as integer

CALL_long_to_int MACRO function
        CALL_void function
        STOSW
        XCHG    AX,DX
        STOSW
coffset=-4
        ENDM

;------ Call C-function, return double as real

CALL_double_to_real MACRO function
        MOV     DS,DX                   ;Now SS and DS points to DGROUP
ASSUME  DS:DGROUP
        MOV     [_csp].segw,CX          ;Save COMAL stack segment
;
        ccall   function                ;Call C-function
;
        MOV     ES,[_cds].segw
ASSUME  ES:data
        MOV     SS,[_csp].segw          ;Reload COMAL stack segment
ASSUME  SS:NOTHING
        MOV     SP,[_csp].offw
        XCHG    SI,AX
IF      sizeD
        MOV     DS,DX
ASSUME  DS:NOTHING
ENDIF
        MOVSW                           ;Copy double to evaluation stack
        MOVSW
        MOVSW
        MOVSW
        PUSH    ES
        POP     DS
ASSUME  DS:data
;;      MOV     DI,[etop]               ;Reload evaluation stack pointer
coffset=-8
        ENDM

;------ Call C-function, return char as string

CALL_char_to_str MACRO function
        CALL_void function
        PUSH    DS
        PUSH    AX
        MOV     CX,1
        CALL    create_str
        POP     [SI]
        POP     DS
coffset=-4
        ENDM

;------ Call C-function, return char far * as string

CALL_char_ptr_to_str MACRO function
        CALL_void function
;
;       DX:AX   points to null-terminated string.
;       Find its length, allocate Comal string for it,
;       finally copy it the Comal string.
;
        CALL    pchar_to_str
pstr1=1                            ;pchar_to_str is used
coffset=-4
        ENDM

;------ Call C-function, return STR as STR

CALL_str_to_str MACRO function
        CALL_void function
;
;       DX:AX   points to a Comal style string.
;       Get its length, allocate Comal string for it,
;       finally copy it the Comal string.
;
        CALL    pstr_to_str
pstr2=1                            ;pstr_to_str is used
coffset=-4
        ENDM

;------ Call C-function, return pointer as integer

CALL_ptr_to_int MACRO function
        CALL_void function
        STOSW
        XCHG    AX,DX
        STOSW
coffset=-4
        ENDM

;------ Postprocessing: char far *i -> REF i#

char_ptr_to_ref_int MACRO offset
        PUSH    DS
        movref  DS,BX,offset&I
        MOV     AL,[BX]
        CBW
        CWD
        MOV     [BX].loww,AX
        MOV     [BX].highw,DX
        POP     DS
        ENDM

;------ Postprocessing: unsigned char far *i -> REF i#

uchar_ptr_to_ref_int MACRO offset
        PUSH    DS
        movref  DS,BX,offset&I
        SUB     AX,AX
        MOV     [BX+1],AL
        MOV     [BX+2],AX
        POP     DS
        ENDM

;------ Postprocessing: short far *i -> REF i#

short_ptr_to_ref_int MACRO offset
        PUSH    DS
        movref  DS,BX,offset&I
        MOV     AX,[BX].loww
        CWD
        MOV     [BX].highw,DX
        POP     DS
        ENDM

;------ Postprocessing: unsigned short far *i -> REF i#

ushort_ptr_to_ref_int MACRO offset
        PUSH    DS
        movref  DS,BX,offset&I
        MOV     [BX].highw,0
        POP     DS
        ENDM

;------ Postprocessing: char far *s -> REF s$

char_ptr_to_ref_str MACRO offset
        PUSH    ES
        PUSH    DI
        movref  ES,DI,offset&S
        MOV     BX,DI
        ADD     DI,s
        MOV     CX,-1
        MOV     AL,0
        REPNE   SCASB                   ;Search for end of string
        NEG     CX                      ;Number of bytes to allocate
        DEC     CX
        DEC     CX                      ;CX=length of string
        MOV     ES:[BX].curlen,CX
        POP     DI
        POP     ES
        ENDM

;------ Output in pass 2

outif   MACRO text
IF2
%out    text
ENDIF
        ENDM

;
;------ Define CEXTRN macro
;
CEXTRN  MACRO function
IF      sizeC
EXTRN   _&function&:FAR
ELSE
_TEXT   SEGMENT WORD PUBLIC 'CODE'
ASSUME  CS:_TEXT
EXTRN   _&function&:NEAR
_TEXT   ENDS
ENDIF
        ENDM

;------ Definition of text segment
;
;       CMODULE mymodule   -- Segment begin
;       CENDMODULE         -- End of segment

CMODULE MACRO module

;------ Define CENDMODULE macro

CENDMODULE &MACRO
IF      pstr1
        mchar_to_str
ENDIF
IF      pstr2
        mstr_to_str
ENDIF
_TEXT   &ENDS
        &END
        &ENDM

;------ Define cCINIT macro used by CINIT macro

cCINIT  &MACRO name
IFB     <name>
        PUBLIC M_&module
M_&module PROC FAR
        RET                     ;No name specified: do nothing
M_&module ENDP
ELSE
CBEGIN  name, M_&module
        CALL_void               ;Initialise module
CEND
ENDIF
        &ENDM
;
        TITLE   module
;
INCLUDE module&d.inc
IFDEF   protect
 INCLUDE mscrtos2.inc
ELSE
 INCLUDE mscrtdos.inc
ENDIF
;
ASSUME  SS:NOTHING, DS:data
        ENDM

;------ Definition of initialisation handler: name()

CINIT   MACRO name
        cCINIT name
        ENDM

;------ Definition of signal handler: short name(short err)

CSIGNAL MACRO name
IFB     <name>
        PUBLIC  signals
signals PROC    FAR
        CMP     AL,s_load
        JE      sig_load
 IF     stdmsg?
        CMP     AL,s_discard
        JNE     not_discard
        JMP     sig_discard
not_discard:
 ENDIF
        CMP     AL,s_link
        JE      sig_link
        CMP     AL,s_unlink
        JE      sig_unlink
sigx:   XOR     AX,AX
        RET
;
;------ s_unlink: Close C runtime environment.
;
sig_unlink:
CPROC   exit
        CALL_void               ;Terminate runtime environment
        XOR     AX,AX
CEND
;
;------ s_link: Initialize C runtime environment.
;
sig_link:
CPROC   initcenv
        CALL_void               ;Initialize C runtime environment
        XOR     AX,AX           ;No error
CEND
;
;------ s_load: Save segment selector of Comal evaluation stack;
;               If standard messages, integrate them.
;
sig_load:
        MOV     BX,DGROUP
        MOV     DS,BX
ASSUME  DS:DGROUP
        MOV     [_cds].segw,ES
 IF     stdmsg?
;
;------ s_load: Reserve local error numbers without using a message file.
;
        MOV     AX,SEG ml_prog
        MOV     DS,AX
ASSUME  DS:NOTHING
        CMP     DS:[perr_seg],0
        JNE     already_msg     ;J/ when no message file
        MOV     DS:[perr_seg],SEG cstd ;Local messages
        ADD     DS:[perr_max],(std_te-std_tb)/2  ;Reserve messages for this module
already_msg:
        PUSH    ES
        POP     DS
        JMP     sigx
;
;------ s_discard: Tell Comal that this module does not have a message file.
;
sig_discard:
        MOV     AX,SEG ml_prog
        MOV     DS,AX
ASSUME  DS:NOTHING
        CMP     DS:[perr_seg],SEG cstd
        JNE     already_msg     ;J/ when not standard messages
        MOV     DS:[perr_seg],0 ;No segment
        JMP     already_msg
 ENDIF
signals ENDP
ELSE
CBEGIN  name, signals
        PUSH    AX              ;Push signal type
        CMP     AL,s_load
        JE      sig_load
        CMP     AL,s_link
        JE      sig_link
 IF     stdmsg?
        CMP     AL,s_discard
        JE      sig_discard
 ENDIF
sigx:                           ;Call signal function
        MOV     DS,DX           ;Now SS and DS points to DGROUP
ASSUME  DS:DGROUP
        MOV     [_csp].segw,CX    ;Save COMAL stack segment
;
        ccall                   ;Call C-function
;
        POP     BX
        CMP     BX,s_unlink
        JNE     sigxx           ;J/ when not s_unlink
;
;------ s_unlink: Close C runtime environment.
;
sig_unlink:
        PUSH    AX
        CALL    _exit           ;Terminate runtime environment
        POP     AX
;
sigxx:  MOV     ES,[_cds].segw
ASSUME  ES:data
        MOV     DI,[etop]       ;Reload evaluation stack pointer
        MOV     SS,[_csp].segw  ;Reload COMAL stack segment
ASSUME  SS:NOTHING
        MOV     SP,[_csp].offw
;
        PUSH    ES              ;DS and ES is data
        POP     DS
ASSUME  DS:data
CEND
;
;------ s_load: Save segment selector of Comal evaluation stack.
;
sig_load:
        MOV     SS:[_cds].segw,ES ;Save Comal evaluation stack segment in _cds
 IF     stdmsg?
;
;------ s_load: Reserve local error numbers without using a message file.
;
        MOV     AX,SEG ml_prog
        MOV     DS,AX
ASSUME  DS:NOTHING
        CMP     DS:[perr_seg],0
        JNE     sigx            ;J/ when no message file
        MOV     DS:[perr_seg],SEG cstd ;Local messages
        ADD     DS:[perr_max],(std_te-std_tb)/2  ;Reserve messages for this module
        JMP     sigx
;
;------ s_discard: Tell Comal that this module does not have a message file.
;
sig_discard:
        MOV     AX,SEG ml_prog
        MOV     DS,AX
ASSUME  DS:NOTHING
        CMP     DS:[perr_seg],SEG cstd
        JNE     sigx            ;J/ when not standard messages
        MOV     DS:[perr_seg],0 ;No segment
 ENDIF
        JMP     sigx
;
;------ s_link: Initialize C runtime environment.
;
sig_link:
        CALL    _initcenv       ;Initialize C runtime environment
        JMP     sigx
ENDIF
        ENDM

;------ Set storage model

CMODEL  MACRO smodel
IFNB    <smodel>
smodel  =        1               ;Define model
ENDIF
;
;------ Storage model control
;
;       Model:                        Code pointers:    Data pointers:
;         Small model:                  NEAR            NEAR
;         Compact model:                NEAR            FAR
;         Medium model:                 FAR             NEAR
;         Large model:                  FAR             FAR
;         Huge model:                   FAR             FAR
;
IFDEF  compact
sizeC   =0                      ;Near calls
sizeD   =1                      ;Far data
outif <Compact storage model>
ELSE
 IFDEF medium
sizeC   =1                      ;Far calls
sizeD   =0                      ;Near data
outif <Medium storage model>
 ELSE
  IFDEF large
sizeC   =1                      ;Far calls
sizeD   =1                      ;Far data
outif <Large storage model>
  ELSE
   IFDEF huge
sizeC   =1                      ;Far calls
sizeD   =1                      ;Far data
outif <Huge storage model>
   ELSE
sizeC   =0                      ;Near calls
sizeD   =0                      ;Near data
outif <Small storage model>
   ENDIF
  ENDIF
 ENDIF
ENDIF
        ENDM

;
;----- Macros for defining standard messages.
;
stdmsg?=0                               ;Assume no messages

cmsg1   MACRO n,text
IFB     <text>
stdmsg&n EQU THIS BYTE
ELSE
stdmsg&n DB text
ENDIF
        ENDM

cmsg2   MACRO n
        DW stdmsg&n
        ENDM

CSTDMSG MACRO last                      ;Messages begin
stdmsg?=1                               ;There are standard messages
stdnmsg=last+1                          ;Number of standard messages
cstd    SEGMENT PARA PUBLIC 'data'
std_end  DW std_last-std_end            ;Offset to end of segment
std_nums DW (std_te-std_tb)/2           ;Total number of messages
;
;------- Offsets to messages:
;
std_tb   EQU THIS WORD
std?=0
        REPT last+1
        cmsg2 %std?
std?=std?+1
        ENDM
;
std_te   DW std_last                    ;End of messages
std?=0
        ENDM

CENDSTDMSG MACRO                        ;Messages end
std_last LABEL BYTE
cstd    ENDS
        ENDM

CMSG    MACRO n, text                   ;Message
        REPT n-std?
        cmsg1 %std?,<>
std?=std?+1
        ENDM
        cmsg1 %std?,<text>
std?=std?+1
        ENDM

mchar_to_str MACRO
;
;       Subroutine pchar_to_str
;       =======================
;
;       DX:AX   points to null-terminated string.
;       Find its length, allocate a Comal string for it,
;       finally copy it to the Comal string.
;
pchar_to_str PROC NEAR
        PUSH    DS
        PUSH    AX
        PUSH    DX
        MOV     ES,DX
        XCHG    DI,AX
        MOV     CX,-1
        MOV     AL,0
        REPNE   SCASB                   ;Search for end of string
        NEG     CX                      ;Number of bytes to allocate
        DEC     CX
        DEC     CX                      ;CX=length of string
;
        PUSH    DS                      ;ES:DI = evaluation stack pointer
        POP     ES
        MOV     DI,[etop]
        CALL    create_str              ;DS:SI,CX:=create_str(ES:DI,CX)
        XCHG    AX,DI                   ;Save new stack pointer
        PUSH    DS                      ;ES:DI := DS:SI
        MOV     DI,SI
        POP     ES
;
        POP     DS                      ;DS:SI := DX:AX
        POP     SI
        REP     MOVSB
        POP     DS                      ;DS:=data
        PUSH    DS                      ;ES:=data
        POP     ES
        XCHG    AX,DI                   ;Restore stack pointer
        RET
pchar_to_str ENDP
        ENDM

mstr_to_str MACRO
;
;       Subroutine pstr_to_str
;       ======================
;
;       DX:AX   points to a Comal style string.
;       Get its length, allocate Comal string for it,
;       finally copy it to the Comal string.
;
pstr_to_str PROC NEAR
        PUSH    DS
        MOV     DS,DX
        XCHG    BX,AX
        MOV     CX,DS:[BX].curlen
        PUSH    BX
        PUSH    DS
                                        ;ES:DI = evaluation stack pointer
        CALL    create_str              ;DS:SI,CX:=create_str(ES:DI,CX)
        XCHG    AX,DI
        PUSH    DS                      ;ES:DI := DS:SI
        MOV     DI,SI
        POP     ES
;
        POP     DS                      ;DS:SI := comal_string
        POP     SI
        ADD     SI,s
REP     MOVSB
        XCHG    DI,AX                   ;New top of evaluation stack
        POP     DS                      ;DS:=data
        PUSH    DS                      ;ES:=data
        POP     ES
        RET
pstr_to_str ENDP
        ENDM
.list
