;*******************************************************************************
;       This file contains initialisation and termination code for
;       MicroSoft C programmed modules. It also contains error messages.
;       DOS version.
;*******************************************************************************

_NFILE_ =       20              ; Maximum number of file handles
FOPEN   equ     01H             ; file handle open
FDEV    equ     40H             ; file handle refers to device
FTEXT   equ     80H             ; file handle is in text mode
DOS     EQU     21H             ; DOS interrupt number
DOS_maxpara equ 00002H          ; maximum paragraph     (PSP:0002)
DOS_envp equ    0002cH          ; environment address   (PSP:002c)
DOS_terminate   equ     4cH     ; terminate process with errorcode
DOS_version     equ     30H     ; get version number
DOS_setmem      equ     4aH     ; set size of memory block
DOS_getvector   equ     35H     ; get interrupt vector
DOS_setvector   equ     25H     ; set interrupt vector
DOS_ioctl       equ     44H     ; ioctl for devices
DOS_close       equ     3eH     ; close file handle

MAXSEG_DOS =    20              ; maximum brkctl segments for REAL mode


equate  MACRO name,offs,sz
name    EQU [BP+offs]
coffset=coffset+sz
        ENDM

parmW   MACRO name              ;WORD parameter
equate  name,%coffset,2
        ENDM

parmDP  MACRO name              ;Data pointer parameter
IF      sizeD
equate  name,%coffset,4
ELSE
equate  name,%coffset,2
ENDIF
        ENDM

; Macro for calling another run-time-library function.
; Does a PUSH CS/CALL NEAR in compact/large models, except
; for QuickC.

callcrt MACRO   funcname
ifdef _QC2
        call    funcname
else
  if sizeC
        push    cs
        call    near ptr (funcname)
  else
        call    funcname
  endif
endif
ENDM

_heap_seg_desc  struc
        checksum        dw      ?       ; checksum area
        hflags          dw      ?       ; flags word
        segsize         dw      ?       ; size of segment
        start           dw      ?       ; offset of first heap entry
        rover           dw      ?       ; rover offset
        last            dw      ?       ; offset to end-of-heap marker
        nextseg         dd      ?       ; far pointer to next _heap_seg_desc
        prevseg         dd      ?       ; far pointer to previous _heap_seg_desc
_heap_seg_desc  ends

;
; _heap_seg_desc.hflags bit offsets
;

_HEAP_MODIFY    equ     01h             ; heap segment size can be modified
_HEAP_FREE      equ     02h             ; heap segment may be freed up to OS
_HEAP_NEAR      equ     04h             ; heap segment is part of the near heap
_HEAP_BASED     equ     08h             ; heap segment is part of the based heap
_HEAP_END       equ     0FFFEh          ; End-of-heap flag

;===========================================================================
;
;       Segment definitions
;
;===========================================================================


C_ETEXT SEGMENT WORD PUBLIC 'ENDCODE'
C_ETEXT ENDS

HDR segment byte public 'MSG'
        db      '<<NMSG>>'
stnmsg  label   byte
HDR ends

public  __acrtmsg
__acrtmsg=      9876h

XIFB segment word public 'DATA'
xifbegin label  byte
XIFB ends
XIF segment word public 'DATA' ; far init's
XIF ends
XIFE segment word public 'DATA'
xifend  label   byte
XIFE ends

XIB segment word public 'DATA'
xibegin label   byte
XIB ends
XI segment word public 'DATA'   ; init's
XI ends
XIE segment word public 'DATA'
xiend   label   byte
XIE ends

XOB segment word public 'BSS'
xontab  label   byte            ; start of onexit table
XOB ends
XO segment word public 'BSS'    ; onexit table
XO ends
XOE segment word public 'BSS'
xonend  label   byte
XOE ends

XPB segment word public 'DATA'
xpbegin label   byte            ; end of onexit table
XPB ends
XP segment word public 'DATA'   ; preterm's
XP ends
XPE segment word public 'DATA'
xpend   label   byte
XPE ends

XCB segment word public 'DATA'
xcbegin label   byte
XCB ends
XC segment word public 'DATA'   ; term's
XC ends
XCE segment word public 'DATA'
xcend   label   byte
XCE ends

XCFB segment word public 'DATA'
xcfbegin label  byte
XCFB ends
XCF segment word public 'DATA'  ; far term's
XCF ends
XCFE segment word public 'DATA'
xcfend  label   byte
XCFE ends

;       Messages used by the ms C environment.

MSG segment byte public 'MSG'
;;      dw      0
;;      db      'R6000',13,10,'- stack overflow',13,10,0
        dw      2
        db      'R6002',13,10,'- floating point not loaded',13,10,0
IFDEF   divide_int
        dw      3
        db      'R6003',13,10,'- integer divide by 0',13,10,0
ENDIF
;;      dw      9
;;      db      'R6009',13,10,'- not enough space for environment',13,10,0
        dw      252
        db      13,10,0
        dw      255
        db      'run-time error ',0
MSG ends
PAD segment byte common 'MSG'
        dw      -1              ; message padding marker
; no padding for now;
; MAX padding would be
;       db      114 dup(0)
PAD ends

EPAD segment byte common 'MSG'
        db      -1
EPAD ends

CDATA segment word common 'DATA'
CDATA ends
_DATA segment word public 'DATA'
_DATA ends
CONST   SEGMENT  WORD PUBLIC 'CONST'
;
;       Contains all constants that can only be read. These
;       include floating point constants and segment values
;       for data items declared FAR in the source file or
;       data items that are forced into their own segment
;       by use of the /Gt option.
;
CONST   ENDS

_BSS    SEGMENT  WORD PUBLIC 'BSS'
;
;       Contains all uninitialized static data items except those
;       items explicitly declared as FAR or HUGE items in the
;       source file.
;
_BSS    ENDS

XIQC    SEGMENT WORD COMMON 'DATA'
        PUBLIC __qczrinit               ;* QC -Zr initializer call address
__qczrinit DW 0
XIQC    ENDS

DBDATA  SEGMENT WORD COMMON 'DATA'      ; Debugger Screen swap setup segment
        PUBLIC ___aDBswpflg, ___aDBswpchk, ___aDBrterr
        PUBLIC ___aDBexit, ___aDBptrchk
___aDBswpflg DW 0             ;* Flag == __aDBswpyes if should attempt swapping
___aDBswpchk DW ___aDBretaddr ;* By default points to a model dependent ret
___aDBrterr DW ___aDBretaddr  ;*
___aDBexit DW ___aDBretaddr   ;*  (Report we are exiting)
___aDBcallbk DD 0             ;* Call back address to debugger
___aDBptrchk DW C_ETEXT
DBDATA  ENDS

        public  __aDBdoswp
__aDBdoswp      =       0d6d6h

_STACK_SIZE = 2048              ;Default stack size = 2K

STACK segment para stack 'STACK'
        db      _STACK_SIZE dup (?)    ; default stack size
STACK ends

                                ;; define DGROUP
DGROUP group CDATA,CONST,_BSS,_DATA,XIFB,XIF,XIFE,XIB,XI,XIE,XOB,XO,XOE,XPB,XP,XPE,XCB,XC,XCE,XCFB,XCF,XCFE,HDR,MSG,PAD,EPAD,XIQC,DBDATA,STACK

EMULATOR_DATA  segment para public 'FAR_DATA'
EMULATOR_DATA  ends

EMULATOR_TEXT  segment para public 'CODE'

public __EmDataSeg
__EmDataSeg dw  EMULATOR_DATA

EMULATOR_TEXT  ends

codeOFFSET equ  offset _TEXT:
dataOFFSET equ  offset DGROUP:

public  __acrtused              ; trick to force in startup
        __acrtused = 9876h      ; funny value not easily matched in SYMDEB


page

IFDEF   nullcheck
extrn __nullcheck:NEAR
_nullcheck equ __nullcheck
ENDIF

CDATA segment                   ; floating point setup segment
assume ds:DGROUP

        dw      0               ; force segment to be at least 0's
public __fpinit                 ; public for signal
__fpinit label DWORD
_fpinit equ __fpinit
fpmath  dd      1 dup (?)       ; linking trick for fp
fpdata  dd      1 dup (?)
fpsignal dd     1 dup (?)       ; fp signal message

CDATA ends

_DATA segment
;
;       The default data segment. Initialized global and
;       static data remain in this segment for all storage
;       models, except for data explicitly declared FAR or for
;       data forced into different segments by use of the
;       /Gt option.
;
extrn   _edata:byte             ; end of data (start of bss)
extrn   _end:byte               ; end of bss (start of stack)

;       these are used by DOS C memory management (not used in Windows)

_atopsp label WORD              ; top of stack (heap bottom)
PUBLIC __atopsp
__atopsp DW 0

_aexit_rtn label WORD           ; NEAR pointer
PUBLIC __aexit_rtn
__aexit_rtn DW codeOFFSET __exit

_asizds label WORD              ; DS size (in bytes)
PUBLIC __asizds
__asizds DW 0


        PUBLIC __nheap_desc     ; near heap descriptor
__nheap_desc =$
_heap_seg_desc <0,_HEAP_NEAR OR _HEAP_MODIFY, 0,0,0,0,0,0>
.ERRE   hflags  eq      2       ; flags better be second word
.ERRE   _asizds+2       EQ      __nheap_desc     ; make sure!

; Heap segment limits (for use with QC)

        PUBLIC __aseghi, __aseglo
__aseghi DW 0                   ; highest heap segment so far
__aseglo DW 0                   ; lowest allowable heap segment


PUBLIC __adbgmsg                ; _adbgmsg is set to null;
IF      sizeC                   ; For FORTRAN, _adbgmsg is set to
_adbgmsg label DWORD
__adbgmsg DD 0                  ; point to _FF_DBGMSG in dbginit
ELSE                            ; initializer in dbgmsg.asm
_adbgmsg label WORD
__adbgmsg DW 0
ENDIF

IFDEF   c_file_info
;       special C environment string

public __acfinfo
__acfinfo label BYTE
_acfinfo equ __acfinfo
        cfile   db      '_C_FILE_INFO='
        cfilex  db      0
        cfileln =       cfilex-cfile
ENDIF

_aintdiv label DWORD
PUBLIC __aintdiv
__aintdiv DD 0                  ; divide error interrupt vector save

_fac label QWORD
PUBLIC __fac
__fac DT 0                      ; floating accumulator

errno label WORD
PUBLIC _errno
_errno DW 0                     ; initial error code
_umaskval label WORD
PUBLIC __umaskval
__umaskval DW 0                 ; initial umask value

;=============== following must be in this order

_pspadr label WORD
PUBLIC __pspadr
__pspadr DW 0                   ; psp:0 (far * to PSP segment)
_psp label WORD
PUBLIC __psp
__psp DW 0                      ; psp:0 (paragraph #)

;=============== above must be in this order

;=============== following must be in this order

public __osversion
__osversion label WORD
_osversion equ __osversion
public __dosvermajor
__dosvermajor label BYTE
_dosvermajor equ __dosvermajor
_osmajor label BYTE
PUBLIC __osmajor
__osmajor DB 0
public __dosverminor
__dosverminor label BYTE
_dosverminor equ __dosverminor
_osminor label BYTE
PUBLIC __osminor
__osminor DB 0

;=============== above must be in this order


_osmode label BYTE
PUBLIC __osmode, __dosmode
__osmode DB 0                   ; 0 = real mode
__dosmode = __osmode

public __oserr
__oserr label WORD
_oserr equ __oserr
_doserrno label WORD
PUBLIC __doserrno
__doserrno DW 0                 ; initial DOS error code

_nfile label WORD
PUBLIC __nfile
__nfile DW _NFILE_              ; maximum number of file handles

public __osfile
__osfile label BYTE
_osfile equ __osfile
        db      3 dup (FOPEN+FTEXT) ; stdin, stdout, stderr
        db      2 dup (FOPEN)   ; stdaux, stdprn
        db      _NFILE_-5 dup (0) ; the other 15 handles

__argc label WORD
PUBLIC ___argc
___argc DW 0
__argv label WORD
PUBLIC ___argv
___argv DW 0
environ label WORD
PUBLIC _environ
IF      sizeD
_environ DD 0                   ; environment pointer
ELSE
_environ DW 0                   ; environment pointer
ENDIF

public __pgmptr
__pgmptr label DWORD            ; pointer to program name
_pgmptr equ __pgmptr
        dw      dataOFFSET dos2nam,DGROUP
dos2nam db      0               ; dummy argv[0] for DOS 2.X

; signal related common data

_child label WORD
PUBLIC __child
__child DW 0                    ; flag used to handle signals from child process

;Overlay related data

_ovlflag label BYTE
PUBLIC __ovlflag
__ovlflag DB 0          ;Overlay flag (0 = no overlays)
_intno label BYTE
PUBLIC __intno
__intno DB 0            ;Overlay interrupt value (e.g., 3F)
_ovlvec label DWORD
PUBLIC __ovlvec
__ovlvec DD 0           ;Address of original overlay handler

EXTRN   _cds:DWORD              ;Comal evaluation stack segment
EXTRN   _csp:DWORD              ;Comal processor stack pointer
temp_sp DW      ?               ;Temporary stack pointer (_create_str)
    PUBLIC  __aaltstkovr
IF  sizeC
__aaltstkovr DD -1
ELSE
__aaltstkovr DW -1
ENDIF
STACKSLOP equ   256             ; stack slop for DOS interrupt overhead
        public  STKHQQ          ; used by parasitic heap
STKHQQ  dw      _end+STACKSLOP ; initial value

_DATA ends

                                ; (includes FORTRAN $DEBUG info)

EXTRN   ml_prog:WORD            ;Beginning of interface definition

_TEXT segment word public 'CODE'



assume cs:_TEXT
assume ds:nothing

page
;***
;__NMSG_TEXT(messagenumber) - find message for given message number
;
;Purpose:
;       This routine returns a near pointer to the message associated with
;       messagenumber.  If the message does not exist, then a 0 is returned.
;
;       This routine assumes DS = DGROUP
;
;Entry:
;       ==PASCAL CALLING CONVENTIONS==
;       messagenumber = WORD message number of desired message
;
;Exit:
;       AX      = pointer to message text or 0 if no message exists.
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

PUBLIC __NMSG_TEXT
ife sizeC
__NMSG_TEXT proc NEAR
msgt equ word ptr [bp+4]
else
__NMSG_TEXT proc FAR
msgt equ word ptr [bp+6]
endif
        push    bp
        mov     bp,sp
        push    si
        push    di
        push    ds
        pop     es
        mov     dx,msgt         ; dx = message number
        mov     si,dataOFFSET stnmsg ; start of near messages

tloop:
        lodsw                   ; ax = current message number
        cmp     ax,dx
        je      found           ;   found it - return address
        inc     ax
        xchg    ax,si
        jz      found           ;   at end and not found - return 0
        xchg    di,ax
        xor     ax,ax
        mov     cx,-1
        repne   scasb           ; skip until 00
        mov     si,di
        jmp     tloop           ; try next entry

found:
        xchg    ax,si
        pop     di
        pop     si
        mov     sp,bp
        pop     bp
        ret     2
__NMSG_TEXT ENDP


page
;***
;__NMSGWRITE(messagenumber) - writes message on Comal screen
;
;Purpose:
;       This routine writes the message associated with messagenumber
;       to stderr.
;
;Entry:
;       ==PASCAL CALLING CONVENTIONS==
;       messagenumber = WORD number of desired message
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************


PUBLIC __NMSG_WRITE
ife sizeC
__NMSG_WRITE proc NEAR
msgw equ word ptr [bp+4]
else
__NMSG_WRITE proc FAR
msgw equ word ptr [bp+6]
endif
_NMSG_WRITE equ __NMSG_WRITE
        push    bp
        mov     bp,sp
        push    di
        push    msgw
        call    __NMSG_TEXT     ; find near text pointer
        or      ax,ax
        jz      nowrite         ; don't write anything if not there

        XCHG    DI,AX
nmsg_wlp:
        MOV     AL,ES:[DI]
        TEST    AL,AL
        JZ      nmsg_done
        CALL    outchar         ; Print message on Comal screen.
        INC     DI
        JMP     nmsg_wlp

nmsg_done:

nowrite:
        pop     di
        mov     sp,bp
        pop     bp
        ret     2
__NMSG_WRITE endp
;***
;_astart - start of C-module.
;
;Purpose:
;       Startup routine to initialize C run-time environment.
;       Is called only when module is loaded from disk, not when
;       it is included in a compiled Comal program. It that case,
;       astart2 is called.
;
;Entry:
;       ES      Program segment prefix
;Exit:
;       DX      number of paragraphs needed by this module.
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

public __astart                 ; start address of loaded "C" module
__astart PROC   FAR
setup:
        mov     di,DGROUP
        MOV     SI,SS
        SUB     SI,DI
        MOV     DS,DI           ; establish DS=DGROUP for
        POP     CX              ; The return address
        POP     DX
        and     SP,not 1        ; make even (if not)
        MOV     SS,DI
        add     SP,dataOFFSET _end-2 ; 2 for _asizds limit
assume  ss:DGROUP, ds:DGROUP
        jnc     SPok

        push    ss              ; establish DS=DGROUP for
        pop     ds              ; _FF_MSGBANNER -> _NMSG_WRITE -> _NMSG_TEXT
IF      sizeC
        call    far ptr _FF_MSGBANNER   ; for "\r\nrun-time error " banner
ELSE
        call    _FF_MSGBANNER   ; for "\r\nrun-time error " banner
ENDIF
        xor     ax,ax           ; stack overflow error message (0)
        push    ax
        call    _NMSG_WRITE
        mov     ax,DOS_terminate shl 8 + 255
        int     DOS             ; terminate process with 255

SPok:

        mov     ax,si           ; si = # paragraphs in DGROUP
        shl     ax,1
        shl     ax,1
        shl     ax,1
        shl     ax,1
        dec     ax
        mov     [_asizds],ax    ; save DS size - 1 (in bytes)

;
; Carve an initial near heap out of the bottom of the stack
;

        mov     bx,dataoffset __nheap_desc ; ss:bx = near heap descriptor
        mov     [bx].checksum,ss; save dgroup in near heap desc

        mov     [bx].segsize,sp ; save as segment size

        .ERRE   _HEAP_END -2    ; make sure they're equal
        mov     ax,_HEAP_END    ; get end-of-heap flag
        push    ax              ; into place
        mov     [bx].last,sp    ; pointer to end-of-heap
        not     ax              ; ax = 1 (0-length free entry)
        push    ax              ; first heap entry
        mov     [bx].start,sp   ; init start/rover
        mov     [bx].rover,sp

;
; Now initialize the top of stack location
;

        mov     [_atopsp],sp    ; Top of Stack Region

        PUSH    DX              ; The return address...
        PUSH    CX

;       release extra space to DOS

        MOV     AX,SP
        MOV     CL,4
        SHR     AX,CL
        INC     AX              ; ax = DGROUP + # para in DGROUP
        add     ax,di
        mov     es:[DOS_maxpara],ax ; fix psp:2
        mov     DX,es           ; dx = PSP base
        sub     DX,ax           ; dx = - # para used
        NEG     DX              ; dx # para used
        RET
__astart ENDP

;
;       __astart2
;       =========
;
;       Set some variables when the module is bound to the Comal run file.
;
;       BX      # of paragraphs of stack.
;

public __astart2                ; start address of bound "C" module
__astart2 PROC   FAR
        PUSH    DS
        MOV     AX,DGROUP
        MOV     DS,AX
ASSUME  ds:DGROUP
        MOV     BX,STACK
        SUB     BX,AX           ; Size of DGROUP minus STACK
        MOV     AX,dataOFFSET _end-1 ; 1 for _asizds limit
        AND     AL,0FEH
        MOV     CL,4
        SHL     BX,CL
        DEC     BX
        mov     [_asizds],BX    ; save DS size - 2 (in bytes)
        ADD     BX,AX           ; Size of DGROUP including stack

;
; Carve an initial near heap out of the bottom of the stack
;

        mov     si,dataoffset __nheap_desc ; ss:bx = near heap descriptor
        mov     [si].checksum,bx; save dgroup in near heap desc

        mov     [si].segsize,bx ; save as segment size

        DEC     BX              ; get end-of-heap flag
        DEC     BX
        mov     WORD PTR [BX],_HEAP_END ; into place
        mov     [si].last,bx    ; pointer to end-of-heap
        DEC     BX              ;      1 (0-length free entry)
        DEC     BX
        mov     WORD PTR [BX],1 ; first heap entry
        mov     [si].start,bx   ; init start/rover
        mov     [si].rover,bx

;
; Now initialize the top of stack location
;

        mov     [_atopsp],BX    ; save top of stack
        POP     DS
        RET
__astart2 ENDP

;***
;_initcenv - Initialise C environment.
;
;Purpose:
;       Startup routine to initialize C run-time environment
;
;Entry:
;       ES - Comal's evaluation stack segment.
;Exit:
;
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************
_initcenv PROC  NEAR
;       zero data areas (_BSS and c_common)
        PUSH    CX
        PUSH    DX
        PUSH    ES
;
;       Set _psp to Comal's psp.
;
        MOV     ES,ES:cdata_seg
        MOV     AX,ES:psp_seg
        PUSH    SS
        POP     ES
assume es:DGROUP
        MOV     ES:_psp,AX
;
        cld                     ; set direction flag (up)
        mov     di,dataOFFSET _edata ; beginning of bss area
        mov     cx,dataOFFSET _end ; end of bss area
        sub     cx,di
        xor     ax,ax
        rep     stosb           ; zero bss

;       C segmentation conventions set up here  (DS=SS and CLD)

        push    ss              ; set up initial DS=ES=SS, CLD
        pop     ds
assume ds:DGROUP

        mov     cx,__qczrinit           ;* Get initializer addr
        jcxz    @F                      ;* Is it zero?
        call    cx                      ;* No -- call indirect through
@F:
;       do necessary initialization BEFORE command line processing!

IF      sizeC
        call    FAR PTR _cinit  ; shared by DOS and Windows
ELSE
        call    _cinit          ; shared by DOS and Windows
ENDIF
        push    ss
        pop     ds              ; ds = DGROUP
assume ds:DGROUP, es:nothing
        POP     ES
        POP     DX
        POP     CX

        RET
_initcenv ENDP

page


;***
;_FF_MSGBANNER - writes out first part of run-time error messages
;
;Purpose:
;       This routine writes "\r\nrun-time error " to standard error.
;
;       For FORTRAN $DEBUG error messages, it also uses the _FF_DBGMSG
;       routine whose address is stored in the _adbgmsg variable to print out
;       file and line number information associated with the run-time error.
;       If the value of _adbgmsg is found to be null, then the _FF_DBGMSG
;       routine won't be called from here (the case for C-only programs).
;
;Entry:
;       No arguments.
;
;Exit:
;       Nothing returned.
;
;Uses:
;       AX,BX,CX,DX,ES are destroyed.
;
;Exceptions:
;       None handled.
;
;*******************************************************************************

PUBLIC __FF_MSGBANNER
ife sizeC
_FF_MSGBANNER label near
__FF_MSGBANNER proc NEAR
else
_FF_MSGBANNER label far
__FF_MSGBANNER proc FAR
endif

        push    bp
        mov     bp,sp
        mov     ax,252                  ; "\r\n" to begin error message
        push    ax
        call    _NMSG_WRITE
if sizeC
        cmp     word ptr [__adbgmsg+2],0
else                                    ; not needed for C-only version
        cmp     [__adbgmsg],0
endif
        jz      dbgmsg_inactive
        call    [__adbgmsg]             ; near or far call as appropriate
dbgmsg_inactive:
        mov     ax,255                  ; run-time error message banner
        push    ax
        call    _NMSG_WRITE
        mov     sp,bp
        pop     bp
        ret
__FF_MSGBANNER endp

if      sizeC
global  proc    far
endif

;***
;_cinit - C initialization
;
;Purpose:
;       This routine performs the shared DOS and Windows initialization.
;       The following order of initialization must be preserved -
;
;      (1.      Integer divide interrupt vector setup   )
;       2.      Floating point initialization
;      (3.      Copy ;C_FILE_INFO into _osfile          )
;      (4.      Check for devices for file handles 0 - 4)
;       5.      General C initializer routines
;
;Entry:
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

PUBLIC __cinit
ife sizeC
_cinit label near
__cinit proc NEAR
else
_cinit label far
__cinit proc FAR
endif

assume ds:DGROUP, ss:DGROUP

;       Store DOS major/minor version number
;       This is done in crt0dat.asm rather than in crt0.asm for Windows' sake.
;

        mov     ah,DOS_version
        int     DOS
        mov     [_osversion],ax


;       *** Increase File Handle Count ***
;
;       (1) This code only works on DOS Version 3.3 and later.
;       (2) This code is intentially commented out; the user must enable
;       this code to access more than 20 files.
;
;       mov     ah,67h          ; system call number
;       mov     bx,_NFILE_      ; number of file handles to allow
;       callos                  ; issue the system call
;       ;check for error here, if desired (if carry set, AX equals error code)
;
;       *** End Increase File Handle Count ***

IFDEF   divide_int

;       1.      Integer divide interrupt vector setup

        mov     ax,DOS_getvector shl 8 + 0
        int     DOS             ; save divide error interrupt
        mov     word ptr [_aintdiv],bx
        mov     word ptr [_aintdiv+2],es

        push    cs
        pop     ds
assume ds:nothing
        mov     ax,DOS_setvector shl 8 + 0
        mov     dx,codeOFFSET __cintDIV
        int     DOS             ; set divide error interrupt
        push    ss
        pop     ds
assume ds:dgroup
ENDIF

;       2.      Floating point initialization

ife     sizeC+sizeD
        cmp     word ptr [fpmath], 0        ; Note: make sure offset __fpmath != 0
        je      nofloat_i

        mov     word ptr [fpmath+2], cs     ; fix up these far addresses
        mov     word ptr [fpsignal+2], cs   ; in the small model math libs

else    ;not memS
        mov     cx,word ptr [fpmath+2]
        jcxz    nofloat_i
endif   ;not memS

        mov     es,[_psp]       ; psp segment
        mov     si,es:[DOS_envp] ; environment segment
        lds     ax,[fpdata]     ; get task data area
assume ds:nothing
        mov     dx,ds           ;   into dx:ax
        xor     bx,bx           ; (si) = environment segment
        call    [fpmath]        ; fpmath(0) - init
        jnc     fpok

        push    ss              ; restore ds from ss
        pop     ds
        jmp     _fptrap         ; issue "Floating point not loaded"
                                ; error and abort

fpok:
        lds     ax,[fpsignal]   ; get signal address
assume ds:nothing
        mov     dx,ds
        mov     bx,3
        call    [fpmath]        ; fpmath(3) - set signal address
        push    ss
        pop     ds
assume ds:DGROUP

nofloat_i:

IFDEF   c_file_info

;       3.      Copy ;C_FILE_INFO into _osfile

;       fix up files inherited from child using ;C_FILE_INFO

        mov     es,[_psp]       ; es = PSP
        mov     cx,word ptr es:[DOS_envp] ; es = user's environment
        jcxz    nocfi           ;   no environment !!!
        mov     es,cx
        xor     di,di           ; start at 0

cfilp:
        cmp     byte ptr es:[di],0 ; check for end of environment
        je      nocfi           ;   yes - not found
        mov     cx,cfileln
        mov     si,dataOFFSET cfile
        repe    cmpsb           ; compare for ';C_FILE_INFO'
        je      gotcfi          ;   yes - now do something with it
        mov     cx,07FFFh       ; environment max = 32K
        xor     ax,ax
        repne   scasb           ; search for end of current string
        jne     nocfi           ;   no 00 !!! - assume end of env.
        jmp     cfilp           ; keep searching

;       found ;C_FILE_INFO and transfer info into _osfile

gotcfi:
        push    es
        push    ds
        pop     es              ; es = DGROUP
        pop     ds              ; ds = env. segment
assume ds:nothing
assume es:dgroup
        mov     si,di           ; si = startup of _osfile info
        mov     di,dataOFFSET _osfile ; di = _osfile block

        lodsb                   ; must be less than 20
        cbw
        xchg    cx,ax           ; cx = number of entries

osflp:
        lodsb                   ; get next byte
        inc     al
        jz      saveit          ; was FF - save as 00
        dec     ax              ; restore al
saveit:
        stosb
        loop    osflp           ; transfer next character

        push    ss
        pop     ds              ; ds = DGROUP
assume ds:dgroup
nocfi:


;       4.      Check for devices for file handles 0 - 4
;
;               Clear the FDEV bit (which might be inherited from C_FILE_INFO)
;               and then call DOS to see if it really is a device or not
;
        mov     bx,4

devloop:
        and     _osfile[bx],not FDEV ; clear FDEV bit on principal

        mov     ax,DOS_ioctl shl 8 + 0 ; issue ioctl(0) to get dev info
        int     DOS
        jc      notdev

        test    dl,80h          ; is it a device ?
        jz      notdev          ;   no
        or      _osfile[bx],FDEV ;   yes - set FDEV bit

notdev:
        dec     bx
        jns     devloop
ENDIF

;       5.      General C initializer routines

        mov     si,dataOFFSET xifbegin
        mov     di,dataOFFSET xifend
if      sizeC
        call    initterm        ; call the far initializers
else
        call    farinitterm     ; call the far initializers
endif

        mov     si,dataOFFSET xibegin
        mov     di,dataOFFSET xiend
        call    initterm        ; call the initializers

        ret
__cinit endp

;***
;exit(status), _exit(status) - C termination
;
;Purpose:
;       The termination sequence is more complicated due to the multiple
;       entry points - exit(code) and _exit(code).  The _exit() routine
;       is a quick exit routine that does not do certain C exit functions
;       like stdio buffer flushing and onexit processing.
;
;       exit (status):
;
;       1.      call runtime preterminators
;
;       _exit (status):
;
;       2.      perform C terminators
;      (3.      perform _nullcheck() for null pointer assignment)
;       4.      close all open files
;       5.      terminate floating point
;      (6.      reset divide by zero interrupt vector)
;       7.      restore int 3F handler
;       8.      terminate with return code to DOS
;
;Entry:
;       int status - exit status
;
;Exit:
;       returns to DOS.
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

public  _exit, _main
ife sizeC
_dummy1 proc NEAR
_exit LABEL NEAR
status equ word ptr [bp+4]
else
_dummy1 proc FAR
_exit LABEL FAR
status equ word ptr [bp+6]
endif
_main = _exit
        push    bp
        mov     bp,sp
assume ds:DGROUP, ss:DGROUP
        PUSH    SS
        POP     DS

;       1.      call runtime preterminators
;               - onexit processing
;               - flushall
;               - rmtmp

        mov     si,dataOFFSET xontab ; beginning of onexit table
        mov     di,dataOFFSET xonend ; end of onexit table
        call    initterm

        mov     si,dataOFFSET xpbegin ; beginning of pre-terminators
        mov     di,dataOFFSET xpend ; end of pre-terminators
        call    initterm


        jmp     short exiting   ; jump into _exit

_dummy1 endp

public  __exit
__exit:
ife sizeC
_dummy2 proc NEAR
status equ word ptr [bp+4]
else
_dummy2 proc FAR
status equ word ptr [bp+6]
endif

        push    bp
        mov     bp,sp
assume ds:DGROUP, ss:DGROUP

exiting:

;       2.      perform C terminators

        mov     si,dataOFFSET xcbegin
        mov     di,dataOFFSET xcend
        call    initterm        ; call the terminators


        mov     si,dataOFFSET xcfbegin
        mov     di,dataOFFSET xcfend
if      sizeC
        call    initterm        ; call the far terminators
else
        call    farinitterm     ; call the far terminators
endif

IFDEF   nullcheck

;       3.      perform _nullcheck() for null pointer assignment

        call    _nullcheck      ; check data in NULL data segment at DS:0
        or      ax,ax           ; zero if no null ptr assignment detected
        jz      startclose
        cmp     status,0        ; zero if no other error has occurred
        jnz     startclose
        mov     status,255      ; nonzero status to indicate an
                                ; null-pointer-assignment error
startclose:
ENDIF

;       4.      close all files (except pre-opened handles 0-4)

        mov     cx,_NFILE_-5
        mov     bx,5            ; close handles that are not pre-opened

closeloop:
        test    _osfile[bx],FOPEN
        jz      notopen

        mov     ah,DOS_close
        int     DOS

notopen:
        inc     bx
        loop    closeloop

;       5.      terminate floating point
;       6.      reset divide by zero interrupt vector
;       7.      restore int 3F handler

        call    _ctermsub       ; fast cleanup

;       8.      return to the DOS

;;      mov     ax,status       ; get return value
;;      mov     ah,DOS_terminate
;;      int     DOS             ; exit with al = return code

        MOV     SP,BP
        POP     BP
___aDBretaddr:
        RET
_dummy2 ENDP

if      sizeC
global  endp
endif

;***
;_ctermsub - more C termination code
;
;Purpose:
;       This routine
;               (1) performs floating-point termination
;             ( (2) resets the divide by zero interrupt vector )
;               (3) restore int 3F handler
;
;Entry:
;
;Exit:
;
;Uses:
;       AX,BX,CX,DX.
;
;Exceptions:
;
;*******************************************************************************

public __ctermsub
__ctermsub label NEAR
_ctermsub equ __ctermsub

;       5.      terminate floating point
        mov     cx,word ptr [fpmath+2] ; test for floating point
        jcxz    nofloat_t       ;   no

        mov     bx,2            ;   yes - cleanup
        call    [fpmath]

nofloat_t:

IFDEF   divide_int
;       6.      reset divide by zero interrupt vector

        push    ds
        lds     dx,[_aintdiv]   ; ds:dx = restore vector
        mov     ax,DOS_setvector shl 8 + 0
        int     DOS             ; set divide error interrupt
        pop     ds
ENDIF

IFDEF   overlays
;       7.      restore overlay interrupt vector

        cmp     byte ptr [_ovlflag],0   ; Overlays in use ??
        jz      done_ovlvec             ; if 0, don't restore overlay vector
        push    ds                      ; save ds
        mov     al,byte ptr [_intno]    ; overlay interrupt number
        lds     dx,dword ptr [_ovlvec]  ; original ovl interrupt vector
        mov     ah,DOS_setvector
        int     DOS                     ; restore the overlay vector
        pop     ds                      ; restore ds
done_ovlvec:
ENDIF
        ret


;***
;initterm - do a set of initializers or terminators
;
;Purpose:
;       The initializors and terminators may be written in C
;       so we are assuming C conventions (DS=SS, CLD, SI and DI preserved)
;       We go through them in reverse order for onexit.
;
;Entry:
;       SI      = start of procedure list
;       DI      = end of procedure list
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

initterm:
        cmp     si,di           ; are we done?
        jae     itdone          ;   yes - no more

if      sizeC
        sub     di,4
        mov     ax,[di]
        or      ax,[di+2]
        jz      initterm        ; skip null procedures
        call    dword ptr [di]
else
        dec     di
        dec     di
        mov     cx,[di]
        jcxz    initterm        ; skip null procedures
        call    cx
endif
        jmp     initterm        ; keep looping

itdone:
        ret


ife     sizeC                   ; S/C models only
;***
;farinitterm - do a set of far initializers or terminators
;
;Purpose:
;       The initializors and terminators may be written in C
;       so we are assuming C conventions (DS=SS, CLD, SI and DI preserved)
;       We go through them in reverse order for onexit.
;
;Entry:
;       SI      = start of procedure list
;       DI      = end of procedure list
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

farinitterm:
        cmp     si,di           ; are we done?
        jae     faritdone       ;   yes - no more

        sub     di,4
        mov     ax,[di]
        or      ax,[di+2]
        jz      farinitterm     ; skip null procedures
        call    dword ptr [di]
        jmp     farinitterm     ; keep looping

faritdone:
        ret
endif   ;sizeC


;***
;_fptrap - trap for missing floating-point software
;
;Purpose:
;       Catches these cases of incomplete f.p. software linked into a program.
;
;       (1) 87.LIB chosen, but no coprocessor present;
;           (i.e., emulator not linked)
;
;       (2) "%e", "%f", and "%g" i/o conversion formats specified, but
;           not all conversion software has been linked in, because the
;           program did not use any floating-point variables or expressions.
;
;Entry:
;       None.
;
;Exit:
;       None.
;
;Uses:
;       AX.
;
;Exceptions:
;       Transfers control to _amsg_exit which ...
;       - Writes error message to standard error:  "floating point not loaded";
;       - Terminates the program by calling _exit().
;*******************************************************************************

public __fptrap
__fptrap label NEAR
_fptrap equ __fptrap
        mov     ax,2            ; issue floating point not loaded
        jmp     __amsg_exit     ;   and die



ASSUME  CS:_TEXT,SS:DGROUP, DS:DGROUP, ES:data
;
;
;       C: runerr(unsigned char err)
;       ============================
;
;       Issue error message to comal.
;
        PUBLIC  _runerr
IF      sizeC
_runerr PROC    FAR
ELSE
_runerr PROC    NEAR
ENDIF
        MOV     BP,SP
        MOV     AL,[BP+4]               ;Get error code
_issue_code:
        MOV     SS,[_csp].segw          ;Reload COMAL stack segment
        MOV     SP,[_csp].offw
        JMP     runerr                  ;Issue error inside Comal
_runerr ENDP

;
;       _cintDIV
;       ========
;
;       Integer divide by zero. Issue Comal error 3: "Division by zero"
;
;       This entry point is jumped to when there has been an attempt
;       to divide by zero.
;
        PUBLIC  __cintDIV
__cintDIV PROC   NEAR
        MOV     AX,3                    ;"Division by zero"
        JMP     _issue_code
__cintDIV ENDP

;
;       _amsg_exit
;       ==========
;
;       Exit the program with error code of 255 and appropriate error
;       message.
;
;       AX      Error number.
;
;       The number is not translated into Comal ditto yet.
;
        PUBLIC __amsg_exit
__amsg_exit =_issue_code

;
;_chkstk - check stack upon procedure entry
;
;Purpose:
;       Provide stack checking on procedure entry.
;
;Entry:
;       AX      = size of local frame
;
;Exit:
;       SP      = new stackframe if successful
;
;Uses:
;       BX, CX, DX
;
;Exceptions:
;       Gives out of memory error if there is not enough
;       stack space for the routine.
;*******************************************************************************

if      sizeC
        PUBLIC  __chkstk, __aFchkstk
__chkstk PROC    FAR
__aFchkstk LABEL FAR
        pop     cx              ; get return offset
        pop     dx              ; get return segment
else
        PUBLIC  __chkstk, __aNchkstk
__chkstk PROC    NEAR
__aNchkstk LABEL NEAR
        pop     cx              ; get return offset
endif
        mov     bx,sp
        sub     bx,ax           ; new position
        jc      OMerr           ; error - out of memory
        cmp     bx,[STKHQQ]     ; SP - AX : STKHQQ (for heap/stack)
        jb      OMerr           ;   error - out of memory

        mov     sp,bx           ; set new stack pointer

if      sizeC
        push    dx              ; push segment
        push    cx              ; push offset
chkproc proc    far
        ret                     ; far return to dx:cx
chkproc endp
else
        jmp     cx              ; return to cx
endif

OMerr:
        MOV     ax,52           ;"Out of stack space"
        jmp     _issue_code     ; give stack overflow and die
__chkstk ENDP


IFDEF   fastcall                ;Define fastcall when using -Gr or _fastcall
;***
;_rchkstk - check stack upon procedure entry (saves registers)
;
;Purpose:
;       Provides support for automatic stack checking in C procedures
;       when stack checking is enabled.  The check stack routines built
;       by this module save ALL registers across call (except CX).
;       This routine is used when the user specifies the -Gr
;       or _fastcall option.
;
;       [LLIBCDLL.LIB NOTE: Unlike other LLIBCDLL routines, DS != DGROUP
;       when chkstk() is called.]
;
;Entry:
;       CX = size of local frame
;
;Exit:
;       SP = new stackframe if successful
;
;Uses:
;       *** Preserves all registers except CX ***
;
;Exceptions:
;       Gives out of memory error and aborts if there is not enough
;       stack space for the routine.
;
;*******************************************************************************

 IF      sizeC
        PUBLIC __aFrchkstk
__aFrchkstk PROC FAR
 ELSE
        PUBLIC __aNrchkstk
__aNrchkstk PROC NEAR
 ENDIF

;
; --- Calculate the new SP ---
;

        push    si              ; save si/di around calculation
        push    di

        mov     di,sp           ; di = sp
        sub     di,cx           ; di = new position
        jc      OMerr           ; error - out of stack

        cmp     di,[STKHQQ]     ; new position ok ?
        jb      stack_err       ; nope - out of stack
;
; --- New SP value is ok ---
; Move the return values down to where the new SP will be, restore bx,
; and return.  [NOTE: Be careful to always keep SP at the bottom of the
; stack in case an interrupt occurs during this sequence.]
;
; di = new SP
;

        mov     si,sp           ; si = old sp
        mov     sp,di           ; sp = final value
        jcxz    done            ; no moves needed if 0 temps

        mov     cx,ss           ; es = ss
        mov     es,cx

        movs   word ptr es:[di],word ptr ss:[si] ; move saved di
        movs   word ptr es:[di],word ptr ss:[si] ; move saved si
        movs   word ptr es:[di],word ptr ss:[si] ; move saved return off
 if sizeC
        movs   word ptr es:[di],word ptr ss:[si] ; move saved return seg
 endif
done:
        pop     di              ; restore di/si
        pop     si
        ret
 IF     sizeC
__aFrchkstk ENDP
 ELSE
__aNrchkstk ENDP
 ENDIF
ENDIF
;
;       outchar(char c)
;       ===============
;
;       Print character to Comal editor.
;
        PUBLIC _outchar
IF      sizeC
_outchar PROC   FAR
        MOV     BX,SP
        MOV     AL,[BX+4]       ;Get character
        JMP     outchar
ELSE
_outchar PROC   NEAR
        MOV     BX,SP
        MOV     AL,[BX+2]       ;Get character
        CALL    outchar
        RET
ENDIF
_outchar ENDP
;
;
;       report(int err,char *errtext,ERR_INFO *e,char file)
;       ===================================================
;
;       Report error and message to Comal.
;
        PUBLIC  _report
IF      sizeC
_report PROC    FAR
coffset=4
ELSE
_report PROC    NEAR
coffset=2
ENDIF
parmW   r_err
parmDP  r_errtext
parmDP  r_err_info
parmW   r_file
        MOV     BP,SP
;
;------ Find length of string:
;
IF      sizeD
        LES     DI,r_errtext
ELSE
        MOV     DI,r_errtext
        PUSH    DS
        POP     ES
ENDIF
        MOV     SI,DI
        MOV     CX,-1
        SUB     AL,AL
        REPNE   SCASB
        NEG     CX                      ;Number of bytes to allocate
        DEC     CX
        DEC     CX                      ;CX=length of string
;
        MOV     DL,r_file               ;File number
        MOV     AX,r_err                ;Error code
IF      sizeD
        PUSH    ES
        POP     DS
        LES     BX,r_err_info
ELSE
        MOV     BX,r_err_info
ENDIF
;
;       Entry:
;         AX:     Error code
;         CX:     Length of error text
;         ES:BX   IF BX <> 0 THEN
;                 Pointer to structure with:
;                   WORD: Error number    (err_number#)
;                   BYTE: Error action    (err_action#)
;                   BYTE: Error class     (err_class#)
;                   BYTE: Error locus     (err_locus#)
;         DS:SI   Address of error text
;         DL:     File number
;
ASSUME  ss:DGROUP, ds:nothing, es:nothing
        MOV     BP,[_csp].offw
        MOV     SS,[_csp].segw          ;Reload COMAL stack segment
        MOV     SP,BP
        JMP     report                  ;Jump into Comal
_report ENDP

;
;
;       STR *create_str(unsigned short length)
;       ======================================
;
;       Create Comal style string.
;       No pointer is left on the evaluation stack.
;
        PUBLIC  _create_str
IF      sizeC
_create_str PROC    FAR
c_length EQU [BP+6]
ELSE
_create_str PROC    NEAR
c_length EQU [BP+4]
ENDIF
ASSUME  SS:DGROUP, DS:DGROUP
        PUSH    BP
        MOV     BP,SP
        PUSH    DI
        MOV     ES,[_cds].segw
        MOV     DI,ES:[etop]
        MOV     CX,c_length             ;Length of string to allocate
;
;       Change stack back to Comal; error handling takes place
;       in Comal automatically.
;
        MOV     [temp_sp],SP
        MOV     SS,[_csp].segw
        MOV     SP,[_csp].offw
;
;       Entry:
;         ES:DI         Comal evaluation stack top
;         CX            Length of string
;
        CALL    create_str
;
;       Exit:
;         DS:SI         Pointer to string
;         ES:DI         New evaluation stack top
;
        LEA     AX,[SI-s] ;DX:AX is result
        MOV     DX,DS
        MOV     BX,DGROUP
        MOV     DS,BX
        MOV     SS,BX
        MOV     SP,[temp_sp]
;
        POP     DI
        POP     BP
        RET
_create_str ENDP

gcmsg   STRUC
err       DW    ?               ;Actual error code
errtext   DB    256 DUP(?)      ;Message text
gcmsg   ENDS
;
;
;       get_comal_msg(unsigned short err,MSG *msg)
;       ==========================================
;
;       Return Comal error message belonging to this module.
;
        PUBLIC  _get_comal_msg
IF      sizeC
_get_comal_msg PROC FAR
coffset=6
ELSE
_get_comal_msg PROC NEAR
coffset=4
ENDIF
parmW   g_err                   ;Local error code
parmDP  g_msg                   ;Pointer to msg structure
ASSUME  SS:DGROUP, DS:DGROUP
        PUSH    BP
        MOV     BP,SP
        PUSH    DI
;
        MOV     AX,g_err        ;Error code local to this module
        MOV     BX,SEG ml_prog  ;Segment of interface definition
        CALL    get_err_msg
IF      sizeD
        LES     DI,g_msg
ELSE
        MOV     DI,g_msg
        PUSH    SS
        POP     ES              ;ES=DGROUP
ENDIF
        MOV     ES:[DI].err,AX  ;Save actual error code
        LEA     DI,[DI].errtext
        REP     MOVSB           ;Copy message text
        MOV     BYTE PTR ES:[DI],0 ;Null terminate
;
        PUSH    SS
        POP     DS              ;DS=DGROUP
;
        POP     DI
        POP     BP
        RET
_get_comal_msg ENDP
;
;
;       char far *getenv(char *varname)
;       ===============================
;
;       Searches environment space for varname.
;       This is a rewrite of the corresponding getenv C library function.
;       It searches Comal's environment space(s). The environment string
;       can be set in the profile file.
;
        PUBLIC  _getenv
IF      sizeC
_getenv PROC FAR
coffset=6
ELSE
_getenv PROC NEAR
coffset=4
ENDIF
parmDP  varname                 ;Pointer to variable to search for
ASSUME  ss:DGROUP, ds:DGROUP
        PUSH    BP
        MOV     BP,SP
        PUSH    SI
        PUSH    DI
;
;------ Find length of string:
;
IF      sizeD
        LES     DI,varname
        PUSH    ES
        POP     DS
ELSE
        MOV     DI,varname
        PUSH    DS
        POP     ES
ENDIF
        MOV     SI,DI
        MOV     CX,-1
        SUB     AL,AL
        REPNE   SCASB
        MOV     BYTE PTR [DI-1],'='     ;Add '=' to the string to search for
        NEG     CX                      ;Number of bytes to allocate
        DEC     CX                      ;CX=length of string
        PUSH    DI
        CALL    scan_env                ;ES:DI := scan_env(DS:SI,CX)
        POP     BX
        MOV     BYTE PTR [BX-1],0       ;Zero terminate
IF      sizeD
        PUSH    SS
        POP     DS
ENDIF
        XCHG    AX,DI
        MOV     DX,ES
;
        POP     DI
        POP     SI
        POP     BP
        RET
_getenv ENDP

        PUBLIC __dataseg
__dataseg DW DGROUP

;
;       cursor(char row, char col)
;       ==========================
;
;       Position cursor at (row,col).
;
        PUBLIC _cursor
IF      sizeC
_cursor PROC    FAR
coffset=6
ELSE
_cursor PROC    NEAR
coffset=4
ENDIF
parmW   p_row
parmW   p_col
set_pos         =27                 ;Adressing relative to current textwindow
set_pos2        =32                 ;Absolute addressing
        PUSH    BP
        MOV     BP,SP
        MOV     DL,BYTE PTR p_col
        MOV     DH,BYTE PTR p_row
        MOV     AH,set_pos2
        CALL    editor_functions    ;Set cursor position
        POP     BP
        RET
_cursor ENDP
