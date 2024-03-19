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
; Source files : MODC.ASM
; Object files : MODC.OBJ, MODCP.OBJ
; Binary files : All modules written in C for Comal (.exe or .dll)
; Version      : 3.02, 3.03, 3.10x, 3.11
; Programmers  : PMP, LNL
; Date         : 1 Apr 1990
; Last changed : 23 Apr 1992
; Requirements : DOS or OS/2, MASM/2
;
; Description  : Load time initialization of a C module for Comal.
;
;                Compile this file with:
;
;                  masm /Mx modc;                 -- DOS
;                  masm /Mx /Dprotect modc,modcp; -- OS/2
;
; Revisions    : 23 Apr 1992; LNL
;                 3 Jun 1992; LNL
;
;-----------------------------------------------------------------------------
;
;
IFDEF   protect
         .286c
ENDIF
;
;
;
mdata   SEGMENT PUBLIC 'data'
mdata   ENDS
;
prog    SEGMENT PUBLIC 'data'
prog    ENDS
;
EXTRN   ml_prog:FAR,    signals:FAR
EXTRN   __astart:FAR    ;Shrink code for C module
IFNDEF  protect
 EXTRN  __astart2:FAR   ;Bound module init
ENDIF
;
;       Data segment used for internal data
;
int_data SEGMENT PARA 'data'
;
;  Record with pointers to div.
;
mlptrs_def STRUC
prog_ptr     DD ?
routines_ptr DD ?
signal_ptr   DD ?
handle_ptr   DD ?
mlptrs_def ENDS
;
ml_ptrs DD      ml_prog         ;Interface description segment
        DD      ml_routines     ;Jump table of Comal supplied routines
        DD      signals         ;The module's signal handler
        DD      ml_handle       ;DOS: PSP, OS/2: DLL handle
;
        PUBLIC  ml_routines, ml_handle
;
ml_handle       DW      ?       ;Handle used for freeing ML module
;
int_data ENDS
;
;
;
code    SEGMENT PARA PUBLIC 'CODE'
        ASSUME  CS:code,DS:NOTHING
;
;
;       Subroutine start
;       ================
;
;       This routine is called as the first thing done after the .EXE module
;       or dynamic link library has been loaded.
;
;       DOS:    First the environment variable block is freed, then
;               standard file handles are closed. Finally, 8 bytes
;               supplied as command line parameter are treated as an address
;               of a far pointer variable to store the address of the
;               INIT routine. The INIT routine returns a pointer to
;               a table of information needed by Comal.
;
;               The __astart routine is called when the .EXE module
;               is loaded. It shrinks the module to the needed size,
;               saves some loader information.
;
;               When the .EXE module is included as part of a compiled
;               program, the START routine is NOT called. The INIT routine
;               is called directly by the compiled program, when it activates
;               the module.
;
;       OS/2:   The __astart routine is called when an instance of
;               the module is created. It saves the status information
;               from the loader in the automatic data segment.
;
;               The only exported name is INIT, which is accessed and
;               called from Comal. It returns a pointer to a table
;               with information needed by Comal.
;
;               The INIT routine returns a non-zero value for successful
;               initialisation.
;
;       The jump table with subroutine calls is automatically
;       set up by Comal after the INIT routine has been called.
;
; DOS   Entry:
;
;       Exit:
;         None                  Terminates
;
; OS/2  Entry:
;
;       Exit:
;         AL                    1: succesful initialisation
;
start   PROC    FAR
;
IFDEF   protect
;
;------ OS/2
;
        CALL    __astart                ;Save start up variables.
        MOV     AL,1                    ;Init was a succes
        RET
;
ELSE
;
;------ DOS
;
psp_offset      EQU     80H
;
        MOV     AX,SEG ml_handle
        MOV     DS,AX
ASSUME  DS:SEG ml_handle
;
; Clean up a little
;
        MOV     [ml_handle],ES  ;Program prefix segment
        MOV     ES,ES:[2CH]
        MOV     AH,49H          ;Free the block of environment variables
        INT     21H
        JC      err_call
;
        MOV     BX,4
close_next:
        MOV     AH,3EH          ;Close all standard file-handles
        INT     21H
        JC      err_call
        DEC     BX
        JNS     close_next
;
; Read command line
;
        PUSH    DS
        POP     ES
        MOV     SI,psp_offset
        MOV     DS,[ml_handle]
ASSUME  DS:nothing,ES:SEG ml_handle
        CLD
        LODSW
        CMP     AX,9+256*' '
        JNE     err_call
        CALL    get_to_dx
        MOV     DI,DX
        CALL    get_to_dx
        MOV     DS,DX
        MOV     WORD PTR [DI+0],OFFSET INIT
        MOV     WORD PTR [DI+2],CS
        PUSH    ES
        POP     DS
ASSUME  DS:SEG ml_handle,ES:nothing
;
;  Shrink the program:
;
IF      1
;------ MS C interface: 15 bytes
        MOV     ES,[ml_handle]
        CALL    __astart                ;Shrink code
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
ELSE
;------ Assembler interface: 15 bytes
        MOV     AX,SP
        ADD     AX,15
        MOV     CL,4
        SHR     AX,CL
        SUB     AX,[ml_handle]
        MOV     DX,SS
        ADD     DX,AX
ENDIF
;
        MOV     AX,3100H        ;Return code 0
        INT     21H             ;Terminate - stay resident
;
err_call:
        MOV     AX,4C01H        ;Return code 1
        INT     21H             ;Terminate prog.
;
;
;       Subroutine get_to_dx
;       ====================
;
;       Read four hexadecimal figures from the DOS command line.
;
;       Entry:
;         DS:SI         Points into DOS command line.
;
;       Exit:
;         DX            Crunched number
;         SI            Advanced by 4
;         AX, CX        Not preserved
;
get_to_dx PROC  NEAR
        LODSW
        TEST    AX,NOT 4F4FH
        JNZ     err_call
        AND     AX,0F0FH
        XCHG    AX,DX
        LODSW
        TEST    AX,NOT 4F4FH
        JNZ     err_call
        AND     AX,0F0FH
        MOV     CL,4
        SHL     AX,CL
        OR      DX,AX
        RET
get_to_dx ENDP
;
;
ENDIF
;
start   ENDP
;
;       Subroutine INIT
;       ===============
;
;       This subroutine is called from Comal just after the module
;       has been loaded. It returns a pointer to the jump table.
;
;       Entry:
;         BX:           OS/2: Module handle.
;                       DOS:  =0 : loaded module
;                             <>0: bound module. Min alloc above.
;
;       Exit:
;         DS:SI         ml_ptrs
;         AX            0
;
        PUBLIC  INIT
INIT    PROC    FAR
IFDEF   protect
        MOV     AX,SEG ml_handle
        MOV     DS,AX
ASSUME  DS:SEG ml_handle
        MOV     [ml_handle],BX
ELSE
        TEST    BX,BX
        JZ      init1              ;J/ when loaded module
        CALL    __astart2          ;Set some variables
init1:
ENDIF
        MOV     AX,SEG ml_ptrs
        MOV     DS,AX
        MOV     SI,OFFSET ml_ptrs
        XOR     AX,AX
dummy_l LABEL   FAR
        RET
;
INIT    ENDP
;
code    ENDS
;
;
;
;
;
IFDEF   protect
;
ASSUME  DS:nothing
;
int_data SEGMENT PARA 'data'
;
;
;       Table containing addresses of routines in COMAL
;       ===============================================
;
rout_tbl MACRO rout
rout      DD    dummy_l
         ENDM
;
;       The following lines must be placed in this order !
;
;       Each address will be changed by the COMAL system on loading
;
ml_rout_size DW size_of_routs
;
ml_routines EQU THIS BYTE
rout_tbl ml_runerr
rout_tbl ml_filerr
rout_tbl ml_report
rout_tbl ml_get_err_msg
rout_tbl ml_get_err_num
rout_tbl ml_get_file
rout_tbl ml_set_file
rout_tbl ml_fpadd
rout_tbl ml_fpsub
rout_tbl ml_fpsubr
rout_tbl ml_fpmul
rout_tbl ml_fpdiv
rout_tbl ml_fpdivr
rout_tbl ml_fpint
rout_tbl ml_intfp
rout_tbl ml_fpsqr
rout_tbl ml_fpsin
rout_tbl ml_fpcos
rout_tbl ml_fptgn
rout_tbl ml_fpatn
rout_tbl ml_fplog
rout_tbl ml_fpexp
rout_tbl ml_outchar
rout_tbl ml_bye
rout_tbl ml_create_str
rout_tbl ml_remove_str
rout_tbl ml_dir_entry
rout_tbl ml_check_dir
rout_tbl ml_get_char
rout_tbl ml_put_char
rout_tbl ml_editor_functions
rout_tbl ml_move_from
rout_tbl ml_move_to
rout_tbl ml_link_event
rout_tbl ml_scan_env
rout_tbl ml_set_test_key
rout_tbl ml_set_read_key
;
size_of_routs EQU $-OFFSET ml_routines
;
int_data ENDS
;
ENDIF
;
;
;
;
code2   SEGMENT PARA PUBLIC 'CODE'
;
ASSUME  CS:code2
;
;
;       Code to JMP into routines in COMAL
;       ==================================
;
ml_call PROC    FAR
;
IFDEF   protect
;
;    Module to Comal redirecting sequence.
;
rout_def MACRO rout
          PUBLIC  rout
rout      LABEL   FAR
          PUSH    DS
          PUSH    SEG ml_routines
          POP     DS
          JMP     DS:[ml_&rout]
         ENDM
;
ASSUME  DS:int_data
;
ELSE
;
;       Each address will be changed by the COMAL system on loading
;
rout_def MACRO rout
          PUBLIC  rout
rout      LABEL   FAR
          JMP     dummy_l
         ENDM
;
ml_rout_size DW size_of_routs
;
ml_routines EQU THIS BYTE
;
ENDIF
;
;       The following lines must be placed in this order !
;
rout_def runerr
rout_def filerr
rout_def report
rout_def get_err_msg
rout_def get_err_num
rout_def get_file
rout_def set_file
rout_def fpadd
rout_def fpsub
rout_def fpsubr
rout_def fpmul
rout_def fpdiv
rout_def fpdivr
rout_def fpint
rout_def intfp
rout_def fpsqr
rout_def fpsin
rout_def fpcos
rout_def fptgn
rout_def fpatn
rout_def fplog
rout_def fpexp
rout_def outchar
rout_def bye
rout_def create_str
rout_def remove_str
rout_def dir_entry
rout_def check_dir
rout_def get_char
rout_def put_char
rout_def editor_functions
rout_def move_from
rout_def move_to
rout_def link_event
rout_def scan_env
rout_def set_test_key
rout_def set_read_key
;
IFNDEF  protect
;
size_of_routs EQU $-OFFSET ml_routines
;
ENDIF
;
ml_call ENDP
;
code2   ENDS
;
_DATA   SEGMENT  WORD PUBLIC 'DATA'
;
;       The default data segment. Initialized global and
;       static data remain in this segment for all storage
;       models, except for data explicitly declared FAR or for
;       data forced into different segments by use of the
;       /Gt option.
;
        PUBLIC  _cds, _csp
_cds    DW      0               ;Comal evaluation stack segment
        DW      ?
_csp    DD      ?               ;Comal processor stack pointer

_DATA   ENDS

        END     start
;
