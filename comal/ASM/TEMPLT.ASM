
	TITLE	template
;
;  This file contains a complete package layout, just to fill in or
;  leave out, as needed. (Ver. 3.00/3.11)
;
INCLUDE templtd.inc
INCLUDE modml.inc
;
code	SEGMENT PARA PUBLIC 'code'
	ASSUME	CS:code, DS:data, ES:data
;
;
;
;  Signal handler for this module.
;
;  AX:		Signal code
;
	PUBLIC	signals
signals PROC	FAR
	XOR	AX,AX
	RET
signals ENDP
;
;
;  Initialisation of templt:
;
	PUBLIC	M_templt
M_templt PROC FAR
;
	;Initialize
;
	RET
M_templt ENDP
;
;
;  Implementation of procedure & function bodies
;  =============================================
;
;
;  PROC templt1(x#,y,REF path$)
;
	PUBLIC	P_templt1
P_templt1 PROC	FAR
	RET
P_templt1 ENDP
;
;
;  FUNC templt2#
;
	PUBLIC	F_templt2I
F_templt2I PROC  FAR
	MOV	AX,10
	STOSW			 ;Return 10
	MOV	AX,0
	STOSW
	RET
F_templt2I ENDP
;
code	ENDS
	END
