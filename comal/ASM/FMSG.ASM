
	TITLE	fmsg
;
;------ This module demonstrates how to include standard messages
;	within a module, so that it contains messages if no message
;	file is present in the language set up by COUNTRY in the
;	profile file for Comal. Standard messages in modules is a
;	facility introduced with Comal version 3.02.
;
;	These messages can be reached from Comal the following way:
;
;	USE fmsg
;	FOR m#:=0 TO 2 DO
;	  PRINT m#,":";ERRTEXT$(fmsg.ERR(m#))
;	ENDFOR m#
;
;	There is a message file in danish to override the standard messages,
;	when COUNTRY in the profile file is "DK":
;
;	COMALDK.TXT	is the source text for the messages in danish.
;	COMALDK.MSG	is the binary file containing messages. It is this
;			file which is loaded by Comal.
;
;	The message file is generated using the utility, GENMSG, on the
;	system diskette:
;
;	GENMSG COMALDK;   (COMALDK.TXT -> COMALDK.MSG)
;
;	The standard messages are specified using macros:
;
;	CSTDMSG last_message_number
;	 {CMSG no,text}
;	CENDSTDMSG
;
;	It is allowed to leave out message numbers, but the messages
;	must be written in numeric order.
;
.xlist
INCLUDE fmsgd.inc
INCLUDE modml.inc
.list
;
;------ Define 3 standard messages.
;
CSTDMSG 2
  CMSG 0,'Module fmsg - version 1.0 for Comal 3.02'
  CMSG 1,'This is message #1 ...'
  CMSG 2,'... and message #2'
CENDSTDMSG
;
code	SEGMENT PARA PUBLIC 'code'
	ASSUME	CS:code, DS:data, ES:data
;
;
;	Subroutine standard_messages
;	============================
;
;	Use standard messages, if no message file is present for this
;	module.
;
;	Entry:
;	  AX	signal type.
;
;	Entry:
;	  AX	preserved.
;
standard_messages PROC NEAR
	PUSH	AX
	CMP	AX,s_load
	JNE	sig_1		;J/ when not s_load
;
;------ s_load: reserve local error numbers without using a message file
;
	PUSH	DS
	MOV	AX,SEG ml_prog
	MOV	DS,AX
ASSUME	DS:NOTHING
	CMP	DS:[perr_seg],0
	JNE	already_msg	;J/ when no message file
	MOV	DS:[perr_seg],SEG cstd ;Local messages
	ADD	DS:[perr_max],(std_te-std_tb)/2  ;Reserve messages for this module
already_msg:
	POP	DS
ASSUME	DS:data
	JMP    SHORT sig_end
;
sig_1:	CMP	AX,s_discard
	JNE	sig_end 	;J/ when not s_discard
;
;------ s_discard: Tell Comal that this module does not have a message file.
;
	PUSH	DS
	MOV	AX,SEG ml_prog
	MOV	DS,AX
ASSUME	DS:NOTHING
	CMP	DS:[perr_seg],SEG cstd
	JNE	sig_2		;J/ when not standard messages
	MOV	DS:[perr_seg],0 ;No segment
sig_2:	POP	DS
ASSUME	DS:data
;
sig_end:POP	AX
	RET
standard_messages ENDP
;
;
;	Signal handler for this module
;	==============================
;
;	In order to use standard messages, call the subroutine
;	standard_messages from the signal handler.
;
;	Entry:
;	  AX		Signal code
;	  DS,ES 	Comal evaluation stack.
;
;	Exit:
;	  AX		0
;
	PUBLIC	signals
signals PROC	FAR
	CALL	standard_messages	;Handle standard messages
	XOR	AX,AX
	RET
signals ENDP
;
;
;  Initialisation of fmsg:
;
	PUBLIC	M_fmsg
M_fmsg	PROC FAR
	RET				;Do nothing
M_fmsg	ENDP
;
code	ENDS
	END
