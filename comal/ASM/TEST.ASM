
; Version 3.00

TITLE	test_package

INCLUDE testd.inc
INCLUDE modml.inc
;
code	SEGMENT PARA PUBLIC 'code'
ASSUME	CS:code, DS:data, ES:data
;
;
;
;	Initializing the MODULE test
;	============================
;
	PUBLIC	M_test
M_test	PROC	FAR
	RET
M_test	ENDP
;
;
;
;	Signal handling routine
;	=======================
;
;	Handle signals from COMAL.
;
;	Entry:
;	  AX		Signal number.
;
	PUBLIC	signals
signals PROC	FAR
	XOR	AX,AX		;Return no error
	RET
signals ENDP
;
;
;
;
;	PROC hi
;	  PRINT "Hi, there !"
;	ENDPROC hi
;
hi_txt	DB 'Hi, there !',13,10
hi_txtl =$-hi_txt
;
	PUBLIC	P_hi
P_hi	PROC	FAR
	PUSH	DS
	PUSH	CS			;DS:=CS
	POP	DS
	MOV	CX,hi_txtl
	MOV	SI,OFFSET hi_txt
	CALL	outstring		;outstring(DS:SI,CX)
	POP	DS
	RET
P_hi	ENDP
;
;
;
;	FUNC add#(a#,b#)
;	  RETURN a#+b#
;	ENDFUNC add#
;
	PUBLIC	F_addI
F_addI	PROC	FAR
	LEA	SI,[DI].addI_aI
	LEA	BX,[DI].addI_bI
	LODSW
	ADD	AX,[BX].loww
	STOSW
	LODSW
	ADC	AX,[BX].highw
	STOSW
	JO	add_err 		;J/ overflow
	RET
;
add_err:MOV	AL,2			;"Overflow"
	JMP	runerr
F_addI	ENDP
;
;
;
;	FUNC string$(char$,n#)
;	  IF LEN(char$)<>1 THEN REPORT 1
;	  IF n#<0 OR n#>65535 THEN REPORT 1
;	  RETURN char$*n#
;	ENDFUNC string$
;
	PUBLIC	F_stringS
F_stringS PROC	FAR
	LDS	BX,[DI].stringS_charS	;Load pointer to string into DS:BX
ASSUME	DS:nothing
	CMP	DS:[BX].curlen,1	;Current length must be = 1
	JNE	arg_err
;
	MOV	DL,DS:[BX].s		;Get the character in DL
;
	PUSH	ES
	POP	DS			;Restore DS
ASSUME	DS:data
;
	LEA	SI,[DI].stringS_nI	;Get address of n#
	LODSW
	MOV	CX,AX			;CX:=n#
	LODSW
	TEST	AX,AX			;0<=n#<2^16
	JNZ	arg_err
;
	PUSH	DX
	CALL	create_str		;DS:SI:=create_str(n#)
	POP	AX			;AL:=char$
;
	PUSH	ES
	PUSH	DI			;DI points after string pointer
					; on evaluation stack.
;
	PUSH	DS			;ES:=DS
	POP	ES
ASSUME	DS:nothing, ES:nothing
	MOV	DI,SI			;Let DI point to start of string
;
REP	STOSB				;Fill string with AL=char$
;
	POP	DI			;Restore DI to point after string
;					; pointer on evaluation stack.
	POP	ES
	PUSH	ES			;DS:=ES:=SEG data
	POP	DS
ASSUME	DS:data, ES:data
;
	RET
;
arg_err:MOV	AL,1			;"Argument error"
	JMP	runerr
F_stringS ENDP
;
;
;	PROC zap
;
	PUBLIC	P_zap
P_zap	PROC	FAR
	MOV	AL,57			;Issue "Not implemented"
	JMP	runerr
P_zap	ENDP
;
;
;
;	PROC out(text$,n#)
;	  IF n#<0 OR n#>65535 THEN REPORT 5
;	  FOR i#:=1 TO n# DO PRINT text$,
;	ENDPROC out
;
	PUBLIC	P_out
P_out	PROC	FAR
	CMP	[DI].out_nI.highw,0	;0<=n#<=65535
	JNE	out_err
	MOV	BP,[DI].out_nI.loww	;Get count
	TEST	BP,BP
	JZ	out_done
;
	PUSH	DS
ASSUME	DS:nothing
out_lp:
	LDS	SI,ES:[DI].out_textS	;Get address of string
	MOV	CX,[SI].curlen		;Get length of string
	LEA	SI,[SI].s		;Address of string
	CALL	outstring
	DEC	BP
	JNZ	out_lp
	POP	DS
ASSUME	DS:data
;
out_done:
	RET				;Return to COMAL
;
out_err: MOV	AL,5			;"Value out of range"
	JMP	runerr
P_out	ENDP
;
;
;
;	PROC getscreen(REF screen$)
;
screen_size = 2000*2
;
	PUBLIC	P_getscreen
P_getscreen PROC FAR
	PUSH	ES
	PUSH	DS
ASSUME	DS:nothing, ES:nothing
;
	LES	DI,[DI].getscreen_screenS	;Address of string
	CMP	ES:[DI].maxlen,screen_size	;maxlen>=screen_size
	JB	out_err
	MOV	ES:[DI].curlen,screen_size	;Set length of screen$
;
	LEA	DI,[DI].s		;Point to start of string
;
	MOV	AX,0b000h		;Address of monochrome video RAM
	MOV	DS,AX
	SUB	SI,SI
;
	MOV	CX,screen_size/2
REP	MOVSW				;Move screen to string
;
	POP	DS
	POP	ES
ASSUME	DS:data, ES:data
	RET				;Return to Comal
P_getscreen ENDP
;
;
;
;  PROC setscreen(REF screen$)
;
screen_size = 2000*2
;
	PUBLIC	P_setscreen
P_setscreen PROC FAR
	PUSH	ES
	PUSH	DS
ASSUME	DS:nothing, ES:nothing
;
	LDS	SI,[DI].setscreen_screenS	;Address of string
	CMP	DS:[SI].maxlen,screen_size	;maxlen>=screen_size
	JB	out_err
	MOV	DS:[SI].curlen,screen_size	;Set length of screen$
;
	LEA	SI,[SI].s		;Point to start of string
;
	MOV	AX,0b000h		;Address of monochrome video RAM
	MOV	ES,AX
	SUB	DI,DI
;
	MOV	CX,screen_size/2
REP	MOVSW				;Move screen to string
;
	POP	DS
	POP	ES
ASSUME	DS:data, ES:data
	RET				;Return to Comal
P_setscreen ENDP
;
;
;
;	PROC sum(a#(),REF s#) CLOSED
;	  s#:=0
;	  FOR i#:=MININDEX(a#()) TO MAXINDEX(a#()) DO
;	    s#:+a#(i#)
;	  ENDFOR i#
;	ENDPROC sum
;
	PUBLIC	P_sum
P_sum	PROC	FAR
	PUSH	DS
	LDS	SI,[DI].sum_aI
ASSUME	DS:nothing
	CALL	sum_it
;
	LDS	SI,ES:[DI].sum_sI	;Store result in s#
	MOV	DS:[SI].loww,AX
	MOV	DS:[SI].highw,DX
	POP	DS
	RET				;Return to COMAL
;
;	Sum vector pointed out by DS:SI.
;
;
sum_it	PROC	NEAR
	MOV	CX,[SI].alen1		;Number of elements in vector
	SUB	AX,AX			;Accumulate in DX:AX
	MOV	DX,AX
	LDS	SI,[SI].abase		;Get address of first element
;
	JCXZ	sum_done		;Jump if no elements
sum_lp: ADD	AX,[SI].loww		;Sum integers
	ADC	DX,[SI].highw
	JO	sum_err 		;Jump if overflow
	ADD	SI,SIZE intvar
	LOOPNE	sum_lp			;Continue until last element or
					; end of segment (SI=0)
	JCXZ	sum_done		;Jump if last element
;
	MOV	BX,DS
	ADD	BX,ES:hadd		;Skip to next segment
	MOV	DS,BX
	JMP	sum_lp
;
sum_done:
	RET
;
sum_err:MOV	AL,2			;"Overflow"
	JMP	runerr
;
ASSUME	DS:data
sum_it	ENDP
;
P_sum	  ENDP
;
;
;
;	FUNC freal
;	  RETURN 3.14
;	ENDFUNC freal
;
;
	PUBLIC	F_frealR
F_frealR PROC	FAR
	MOV	WORD PTR [DI].0,34079
	MOV	WORD PTR [DI].2,20971
	MOV	WORD PTR [DI].4,7864
	MOV	WORD PTR [DI].6,16393
	ADD	DI,8			;DI points after returned value
	RET
F_frealR ENDP
;
;
;
;	FUNC fint#
;	  RETURN 23
;	ENDFUNC fint#
;
	PUBLIC	F_fintI
F_fintI PROC	FAR
	MOV	[DI].loww,23
	MOV	[DI].highw,0
	ADD	DI,4			;DI points after returned value
	RET
F_fintI ENDP
;
;
;
;	FUNC fstr$
;	  RETURN "IBM PC"
;	ENDFUNC fstr$
;
	PUBLIC	F_fstrS
F_fstrS PROC	FAR
	PUSH	DS
ASSUME	DS:nothing
	MOV	CX,6
	CALL	create_str		;DS:SI:=create_str(6)
					;DI points after returned value
	MOV	WORD PTR [SI].0,'BI'
	MOV	WORD PTR [SI].2,' M'
	MOV	WORD PTR [SI].4,'CP'
	POP	DS
ASSUME	DS:data
	RET
F_fstrS ENDP
;
;
;
;	PROC monadic(REF x(),operator#)
;	  FOR i#:=MININDEX(x()) TO MAXINDEX(x()) DO
;	    CASE operator# OF
;	    WHEN 1
;	      x(i#):=FLOAT(ROUND(x(i#)))
;	    WHEN 2
;	      x(i#):=SIN(x(i#))
;	    WHEN 3
;	      x(i#):=COS(x(i#))
;	    WHEN 4
;	      x(i#):=SQR(x(i#))
;	    ENDCASE
;	  ENDFOR i#
;	ENDPROC monadic
;
	EVEN
mona	DD mon_rnd, fpsin, fpcos, fpsqr
;
;	result:=FLOAT(ROUND(operand))
;
mon_rnd PROC	FAR
	CALL	fpint			;real -> int
	CALL	intfp			;int -> real
	RET
mon_rnd ENDP
;
	PUBLIC	P_monadic
P_monadic PROC	FAR
	MOV	BX,[DI].monadic_operatorI.highw ;Test operator#
	TEST	BX,BX				;1<=operator#<=4
	JNZ	err
	OR	BX,[DI].monadic_operatorI.loww	;Get operator#
	JZ	err
	CMP	BX,4
	JA	err
;
	SHL	BX,1			;Multiply by 4
	SHL	BX,1
;
ASSUME	DS:nothing
	LDS	SI,[DI].monadic_xR	;Examine vector
	MOV	CX,[SI].alen1		;Get number of elements
	LDS	SI,[SI].abase		;Address of first element
;
	JCXZ	mona_done		;Jump if empty vector
mona_loop:
	MOV	DI,SI
;
;  [DI] := F([SI])
;
trigo:	CALL	mona[BX-4]		;Handle operation
	CMP	numerr,0		;Numeric error ?
	JNZ	math_err2
	ADD	SI,8			;Next real
	LOOPNZ	mona_loop		;Cont. until last real or end of segm.
	JCXZ	mona_done		;Stop if last element
;
	MOV	AX,DS
	ADD	AX,ES:hadd		;Continue in next segment
	MOV	DS,AX
	JMP	mona_loop
;
mona_done:
	PUSH	ES
	POP	DS			;Restore DS=ES=SEG data
ASSUME	DS:data
	RET
P_monadic ENDP
;
math_err2:
	MOV	AL,numerr
	JMP	runerr
;
err:	JMP	arg_err 		;"Argument error"
;
;
;	PROC dyadic(REF x(),REF y(),operator#)
;	  FOR i#:=MININDEX(x()) TO MAXINDEX(x()) DO
;	    CASE operator# OF
;	    WHEN 1
;	      x(i#):=x(i#)+y(i#)
;	    WHEN 2
;	      x(i#):=x(i#)-y(i#)
;	    WHEN 3
;	      x(i#):=x(i#)*y(i#)
;	    WHEN 4
;	      x(i#):=x(i#)/y(i#)
;	    ENDCASE
;	  ENDFOR i#
;	ENDPROC dyadic
;
	EVEN
dyad	DD fpadd,fpsub,fpmul,fpdiv
;
dyad_vars STRUC
s_xptr	   DD	?
s_yptr	   DD	?
s_rout	   DD	?
s_temp1    DQ	?
s_temp2    DQ	?
dyad_vars ENDS
;
xptr EQU [BP].s_xptr
yptr EQU [BP].s_yptr
rout EQU [BP].s_rout
num1 EQU [BP].s_temp1
num2 EQU [BP].s_temp2
;
	PUBLIC	P_dyadic
P_dyadic PROC	FAR
	SUB	SP,SIZE dyad_vars	;Make space for local variables
	MOV	BP,SP			; on processor stack
;
	MOV	BX,[DI].dyadic_operatorI.highw
	TEST	BX,BX			;1<=operator#<=4
	JNZ	err
	OR	BX,[DI].dyadic_operatorI.loww
	JZ	err
	CMP	BX,4
	JA	err
;
	SHL	BX,1			;Multiply by 4
	SHL	BX,1
	MOV	AX,CS:dyad[BX-4].offw
	MOV	rout.offw,AX		;Copy pointer to routine
	MOV	AX,CS:dyad[BX-4].segw
	MOV	rout.segw,AX
;
ASSUME	DS:nothing
	LDS	SI,[DI].dyadic_xR	;Examine vector
	MOV	CX,[SI].alen1		;Get number of elements
	MOV	AX,[SI].abase.offw
	MOV	xptr.offw,AX		;Address of first element
	MOV	AX,[SI].abase.segw
	MOV	xptr.segw,AX
;
	LDS	SI,ES:[DI].dyadic_yR	;Examine vector
	CMP	CX,[SI].alen1		;Check number of elements
	JNE	err
	MOV	AX,[SI].abase.offw
	MOV	yptr.offw,AX		;Address of first element
	MOV	AX,[SI].abase.segw
	MOV	yptr.segw,AX
;
	PUSH	ES
	JCXZ	dya_done		;Jump if empty vector
;
ASSUME	ES:nothing
dya_loop:
	PUSH	SS
	POP	ES
	LEA	DI,num2
	LDS	SI,yptr
	MOVSW				;Move y(i#) to num2
	MOVSW
	MOVSW
	MOVSW
;
	MOV	AX,DS			;Adjust pointer to next element
	TEST	SI,SI
	JNZ	y_store
	POP	DS			;Get SEG data
	PUSH	DS
ASSUME	DS:data
	ADD	AX,DS:hadd
ASSUME	DS:nothing
y_store:
	MOV	yptr.offw,SI
	MOV	yptr.segw,AX
;
	LEA	DI,num1
	LDS	SI,xptr
	MOVSW				;Move x(i#) to num1
	MOVSW
	MOVSW
	MOVSW
;
	PUSH	SS
	POP	DS
	LEA	SI,num2
	LEA	DI,num1
	CALL	rout			;[DI] := [DI] ? [SI]
;
	LEA	SI,num1
	LES	DI,xptr
	MOVSW				;Move result to x(i#)
	MOVSW
	MOVSW
	MOVSW
;
	POP	DS			;Get SEG data
	PUSH	DS
ASSUME	DS:data
	CMP	DS:numerr,0
	JNZ	math_err
;
	MOV	AX,ES			;Adjust pointer to next element
	TEST	DI,DI
	JNZ	x_store
	ADD	AX,DS:hadd
x_store:
ASSUME	DS:nothing
	MOV	xptr.offw,DI
	MOV	xptr.segw,AX
;
	LOOP	dya_loop
;
dya_done:
	POP	ES
	PUSH	ES
	POP	DS
ASSUME	DS:data, ES:data
;
	ADD	SP,SIZE dyad_vars	;Adjust SP back to the original
	RET
P_dyadic ENDP
;
;
math_err:
	MOV	AL,numerr		;Issue error
	JMP	runerr
;
;
;	FUNC solve1(a,b,c)
;	  RETURN (-b+SQR(b*b-4*a*c))/(2*a)
;	ENDFUNC solve1
;
ASSUME	ES:data
;
s_a = solve1R_aR
s_b = solve1R_bR
s_c = solve1R_cR
;
	PUBLIC	F_solve1R
	EVEN
F_solve1R PROC	FAR
;
	MOV	BX,DI
	LEA	DI,[BX].s_c
	LEA	SI,[BX].s_a
	CALL	fpmul			;c:=a*c
;
	MOV	AX,4			;r:=FLOAT(4)
	CWD
	MOV	DI,BX
	CALL	intfp
;
	LEA	SI,[BX].s_c		;r:=FLOAT(4)*a*c
	CALL	fpmul
;
	LEA	SI,[BX].s_b
	LEA	DI,[BX].s_c
	MOV	CX,4
REP	MOVSW
;
	LEA	SI,[BX].s_b		;c:=b*b
	LEA	DI,[BX].s_c
	CALL	fpmul
;
	MOV	SI,BX			;c:=b*b-4*a*c
	CALL	fpsub
;
	MOV	SI,DI
	CALL	fpsqr			;c:=SQR(b*b-4*a*c)
;
	LEA	SI,[BX].s_b		;c:=SQR(b*b-4*a*c)-b
	CALL	fpsub
;
	MOV	DI,BX			;r:=FLOAT(2)
	MOV	AX,2
	CWD
	CALL	intfp
;
	LEA	SI,[BX].s_a		;r:=2*a
	CALL	fpmul
;
	LEA	SI,[BX].s_c		;r:=(-b+SQR(b*b-4*a*c))/(2*a)
	CALL	fpdivr
;
	CMP	numerr,0		;Math error ?
	JNE	math_err
;
	LEA	DI,[BX+8]		;Points after result
;
	RET				;Return
F_solve1R ENDP
;
;
;
;	PROC debug
;	==========
;
;	Execute a INT 1 instruction. This instruction does usually
;	nothing. If the DEBUG-program is active, will an INT 1 instruction
;	cause a call of DEBUG, which gives the possibility of following
;	the execution of a procedure.
;
	PUBLIC	P_debug
P_debug PROC	FAR
	INT	1			;Allow debugging
	MOV	AX,1			;Execute AX:=1+5
	ADD	AX,5
	RET
P_debug ENDP
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
;	  CX		Length of string
;
;	Exit:
;	  CX, SI	Not preserved.
;
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