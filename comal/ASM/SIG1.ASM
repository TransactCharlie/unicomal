
; Version 3.00

TITLE	sig1_module

INCLUDE sig1d.inc
INCLUDE modml.inc
;
code	SEGMENT PARA PUBLIC 'code'
ASSUME	CS:code, DS:data, ES:data
;
;
;
;	Initializing the MODULE sig1
;	============================
;
;	Print
;
;	  sig1.init
;
	PUBLIC	M_sig1
M_sig1	PROC	FAR
	MOV	AX,2*12 	;Treat initialisation as signal 2*12
	CALL	print_signal
	RET
M_sig1	ENDP
;
;
;
;	Signal handling routine
;	=======================
;
;	Print all signals in the form
;
;	  sig1.s_signal
;
;
	PUBLIC	signals
signals PROC	FAR
	CALL	print_signal
	XOR	AX,AX		;Return no error
	RET
signals ENDP
;
;
;	Subroutine print_signal
;	=======================
;
;	Print signal in the form:
;
;	  sig1.s_signal
;
;	Entry:
;	  AX		Signal number.
;
;	Exit:
;	  CX, SI	Not preserved.
;
;
me	DB '  ',16,'sig1.'      ;Name of this module
melen=$-me
;
nl	DB 13,10		;Carriage return
nllen=$-nl
;
sigs	DW	sig0		;Table of string addresses
	DW	sig1
	DW	sig2
	DW	sig3
	DW	sig4
	DW	sig5
	DW	sig6
	DW	sig7
	DW	sig8
	DW	sig9
	DW	sig10
	DW	sig11
	DW	sig12
	DW	sig13
;
sig0	DB	's_load'        ;Issued during CHAIN, RUN, SCAN
sig1	DB	's_discard'     ;Issued by NEW, LOAD, CHAIN, BYE(, RUN, SCAN)
sig2	DB	's_link'        ;Issued during RUN, CHAIN
sig3	DB	's_unlink'      ;Issued before SCAN, RUN, CHAIN, NEW, BYE
sig4	DB	's_stop'        ;Issued by STOP, END or CTRL-Break
sig5	DB	's_con'         ;Issued by CON
sig6	DB	's_error_1'     ;Issued by run time error before the message
sig7	DB	's_error_2'     ;Issued by run time error after the message
sig8	DB	's_pass_1'      ;Issued by PASS before call
sig9	DB	's_pass_2'      ;Issued by PASS after call
sig10	DB	's_reset_video' ;Issued after execution of ALT-O
sig11	DB	's_use'         ;Issued by USE statement
sig12	DB	'init'          ;Not a signal, it is the initialisation
sig13=$
;
print_signal PROC NEAR
	PUSH	DS
	PUSH	CS		;DS:=CS
	POP	DS
ASSUME	DS:code
	PUSH	AX
	MOV	SI,OFFSET me
	MOV	CX,melen
	CALL	outstring	;Print "sig1."
;
	POP	BX
	MOV	SI,sigs[BX]
	MOV	CX,sigs[BX+2]
	SUB	CX,SI
	CALL	outstring	;Print signal name
;
	MOV	SI,OFFSET nl
	MOV	CX,nllen
	CALL	outstring	;New line
;
	POP	DS
ASSUME	DS:data
	RET
print_signal ENDP
;
;
;
;
;
;	Subroutine outstring
;	====================
;
;	Print a string to the screen, using COMAL
;	editor calls.
;
;	Entry:
;	  DS:SI 	Address of string to print.
;	  CX		Length of string.
;
;	Exit:
;	  SI, CX	Not preserved.
;
ASSUME	DS:NOTHING
outstring PROC NEAR
	CLD
	JCXZ	outx		;Jump if no characters
outlp:	LODSB			;Get character
	CALL	outchar
	LOOP	outlp
outx:	RET
outstring ENDP
;
;
code	ENDS
	END
