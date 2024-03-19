;----------------------------------------------------------------------------+
;									     |
;   U	 U	       CCCC			     l	    (C) Copyright    |
;   U	 U	      C 			     l	     UniComal A/S    |
;   U	 U  nnnn   i  C       ooo   mmmmmm    aaaa   l		1989	     |
;   U	 U  n	n  i  C      o	 o  m  m  m  a	 a   l			     |
;   U	 U  n	n  i  C      o	 o  m  m  m  a	 a   l	      80x86 COMAL    |
;    UUUU   n	n  i   CCCC   ooo   m  m  m   aaaaa   ll    ===============  |
;									     |
;----------------------------------------------------------------------------+
;
; Source files : TIMER.ASM, TIMERD.INC, MODML.INC
; Object files : TIMER.OBJ TIMERP.OBJ
; Version      : 3.00
; Programmers  : MK, PP, LNL
; Date	       : 24 Jul 1989
; Last changed : 19 Apr 1990
; Requirements : DOS, OS/2, MASM/2.
;
; Description  : Timer module.
;
; Revisions    :  4 Jul 1989, PP	Template
;		 24 Jul 1989, MK	1st try
;		 30 Nov 1989, LNL	Cleaned.
;		 19 Apr 1990, PP	OS/2.
;		 13 Jun 1990, LNL	Timer_queue proc added.
;
; This is a basic timer module, which can be extended to
; satisfy your need.
;
; It is just included for you to get ideas to make your own module,
; and to make you see how the event processing works.
;
;-----------------------------------------------------------------------------
;
	TITLE	UniTimer_module
;
;	Include timer module declarations, and module declarations in general.
;
INCLUDE timerd.inc
INCLUDE modml.inc

IFDEF	protect
;
;------- Protected mode, i.e. OS/2
;
	.286C		;286 protect mode.
	.287		;80x87 instructions allowed.
ELSE
;
;------- DOS mode.
;
	.8087		;8087 instructions allowed.
ENDIF
;
;------ Limitations on size of queue:
;
queue_limit EQU 10
;
;
;------ Set up action macroes for entering and exiting
;	critical sections:
;
IFDEF	protect
;
;------ OS/2
;
@entercrit MACRO
	PUSH	AX
	CALL	DosEnterCritSec
	POP	AX
	ENDM
;
@exitcrit MACRO
	PUSH	AX
	CALL	DosExitCritSec
	POP	AX
	ENDM
ELSE
;
;------ DOS
;
@entercrit MACRO
	CLI
	ENDM
;
@exitcrit MACRO
	STI
	ENDM
ENDIF
;
;
;====== Local data segment for this module ===================================
;
mdata	SEGMENT PARA PUBLIC 'data'
;
ASSUME	CS:mdata
IFDEF	protect
;
;------ OS/2
;
;	Stack for timer thread
;
	EVEN
cstack	DW	512 DUP(?)
cstack_end equ	$
;
;	A table of event queue elements:
;
event_max DW	queue_limit	;Number of queue elements
event_buf event_entry queue_limit DUP(<,0,>)
;
;	These variables are used during event processing:
;
;	  thandle	<0  :	Timer is inactive.
;			>=0 :	Handle to interval timer.
;	  tinterval	    :	Number of milliseconds between events.
;	  threadid	    :	ID of timer thread
;	  tsem1 	    :	Handle to system semaphore for timer tick.
;	  tsem2 	    :	Semaphore for timer thread termination.
;	  tsem3 	    :	Signal, that timer thread has finished.
;	  semlist	    :	List of semaphores for DosMuxSemWait.
;	  semindex	    :	Index returned for list mentioned above.
;
;	  event_prio	    :	Current priority of events.
;	  event_missed	=0  :	No events missed.
;			<>0 :	Some events have been missed.
;	  timer_proc	    :	Current event handler procedure.
;	  event_max	    :	Number of queue elements
;
;	Furthermore, the variables in the DATA segment:
;
;	  act_event	=0  :	No event is handled.
;	  act_event	<>0 :	Is a pointer to the event_entry of
;				the currently handled event.
;				When this event finishes, the
;				priority field is set to 0 by COMAL.
;	  act_priority	=0  :	No event is handled. This is the
;				priority of running program.
;			<>0 :	The priority of the event given
;				by act_event.
;
thandle 	DW	0FFFFH	;handle for timer
;
tsem2		DD	0	;semaphore for timer thread termination
tsem3		DD	0	;signal, that timer thread has finished
semlist 	DW	2	;two semaphores in list
		DW	0
tsem1		DD	0	;semaphore for timer tick
		DW	0
		DD	tsem2	;semaphore for timer thread termination
semindex	DW	0	;index into semlist
threadid	DW	0	;thread ID
;
tinterval	DD	-1	;timer interval, OS/2: millisecs
;
event_missed	DW	0	;Flag telling whether an event
				; has been missed or not
event_prio	DW	0	;Event priority of event procedure.
timer_proc	DQ	0	;Comal procedure handle for timer event.
;
;	Name of semaphore
;
semname DB	'\SEM\UNITIMER\TIMER'
semno	DB	'000.SEM',0     ;Number to be changed, if semaphore exist.
;
	EVEN
temp	DQ	0.0		;Temporary variable used by set_timer_speed.
;
c1000	DQ	1000.0		;Number of timer units per second
;
ELSE
;
;------ DOS
;
;	A table of event queue elements:
;
event_max DW	queue_limit	;Number of queue elements
event_buf event_entry queue_limit DUP(<,0,>)
;
;	These variables are used during event processing:
;
;	  tinterval	<0  :	Timer is inactive.
;			>=0 :	Number of timer ticks between events.
;	  tcounter	    :	Number of timer ticks before next event.
;	  event_prio	    :	Current priority of events.
;	  event_missed	=0  :	No events missed.
;			<>0 :	Some events have been missed.
;	  timer_proc	    :	Current event handler procedure.
;	  event_max	    :	Number of queue elements
;
;	Furthermore, the variables in the DATA segment:
;
;	  act_event	=0  :	No event is handled.
;	  act_event	<>0 :	Is a pointer to the event_entry of
;				the currently handled event.
;				When this event finishes, the
;				priority field is set to 0 by COMAL.
;	  act_priority	=0  :	No event is handled. This is the
;				priority of running program.
;			<>0 :	The priority of the event given
;				by act_event.
;
tinterval	DD	-1	;Timer interval, DOS ticks
tcounter	DD	0	;Counter
;
event_prio	DW	0	;Event priority of event procedure.
event_missed	DW	0	;Flag telling whether an event
				; has been missed or not
timer_proc	DQ	0	;Comal procedure handle for timer event.
;
temp	DQ	0.0		;Temporary variable used by set_timer_speed.
;
ctick	DQ	18.2064819335938 ;Number of timer ticks per second
;
ENDIF
;
mdata	ENDS
;
;
IFDEF	protect
 EXTRN	DosEnterCritSec:FAR,	DosExitCritSec:FAR,	DosSemSet:FAR
 EXTRN	DosSemClear:FAR,	DosTimerStart:FAR,	DosTimerStop:FAR
 EXTRN	DosCreateThread:FAR,	DosMuxSemWait:FAR,	DosExit:FAR
 EXTRN	DosSemWait:FAR, 	DosCreateSem:FAR,	DosCloseSem:FAR
ENDIF
;
;====== Code segment of timer module =========================================
;
code	SEGMENT PARA PUBLIC 'code'
	ASSUME	CS:code, DS:data, ES:data
;
IFNDEF	protect
;
;------ DOS
;
old_1c	DD	0			;Old INT 1C interrupt vector.
ENDIF
;
;
;	Signals
;	=======
;
;	Handle signals send from Comal for this module.
;
;	The following signals are handled:
;
;	DOS:
;	  s_load	Nothing.
;	  s_link	Link into timer tick interrupt. Enable timer.
;	  s_unlink	Unlink timer tick interrupt. Disable timer.
;	  s_discard	Nothing.
;	OS/2:
;	  s_load	Create timer thread.
;	  s_link	Enable timer.
;	  s_unlink	Disable timer.
;	  s_discard	Stop timer thread.
;
;	Entry:
;	  AX:		Signal code
;
;	Exit:
;	  AX=0		No error
;
	PUBLIC	signals
signals PROC	FAR
	CMP	AL,s_load
	JNE	nsload
	JMP	sload			;J/ when s_load
nsload:
	CMP	AL,s_link
	JNE	nslink
	JMP	slink			;J/ when s_link
nslink:
	CMP	AL,s_unlink
	JNE	nsunlink
	JMP	sunlink 		;J/ when s_unlink
nsunlink:
	CMP	AL,s_discard
	JNE	nsdiscard
	JMP	sdiscard		;J/ when s_discard
nsdiscard:
	XOR	AX,AX			;Return 0: No error
	RET
;
;------ Link, start of program area where this module can be accessed
;	Link into timer interrupt (DOS).
;
slink:
;
;	Reset all entries in event table:
;
	CALL	clear_event_table
IFNDEF	protect
;
;------ DOS - link into timer interrupt.
;
;	Link into INT 1C
;
;	Store original vector away in old_1c.
;
	PUSH	DS
	PUSH	ES
	MOV	AX,351CH
	INT	21H
	MOV	CX,ES
	MOV	AX,SEG mdata
	MOV	DS,AX
ASSUME	DS:mdata
	MOV	CS:old_1c.offw,BX
	MOV	CS:old_1c.segw,CX
;
;	Reset all counters
;
	XOR	AX,AX
	MOV	tcounter.loww,AX	;tcounter:=0
	MOV	tcounter.highw,AX
	DEC	AX
	MOV	tinterval.loww,AX	;tinterval:=-1
	MOV	tinterval.highw,AX
;
;	Insert new int 1C interrupt vector.
;
	MOV	AX,251CH
	PUSH	CS
	POP	DS
ASSUME	DS:code, ES:nothing
	MOV	DX,OFFSET tthread
	INT	21H
	POP	ES
	POP	DS
ASSUME	DS:data, ES:data
ENDIF
	XOR	AX,AX			;Return 0: no error
	RET
;
sload:
;
;------ Load module: Create thread (OS/2)
;
IFDEF	protect
;
;------ OS/2 - create thread.
;
	PUSH	DS
	MOV	AX,SEG mdata
	MOV	DS,AX
ASSUME	DS:mdata
;
;------ Create system semaphore, tsem1, for timer tick.
;
try_next_sem:
	PUSH	1		;No exclusive, to make OS/2 allowed
	PUSH	DS		;to clear the semaphore by means of
	PUSH	OFFSET tsem1	;asynchronious timer !!!
	PUSH	DS
	PUSH	OFFSET semname
	CALL	DosCreateSem
	TEST	AX,AX
	JZ	sem_created
;
;	Try next number, if already exist
;
try_next_num:
	MOV	SI,2
	INC	semno[SI]
	CMP	semno[SI],'9'
	JBE	try_next_sem
	MOV	semno[SI],'0'
	DEC	SI
	JNS	try_next_num
;
;	Set semaphore 2
;
sem_created:
	PUSH	DS
	PUSH	OFFSET tsem2
	CALL	DosSemSet		;Set s2: Do not stop timer thread
;
;------ Create timer thread for asynchronious timer.
;
	PUSH	CS
	PUSH	OFFSET tthread
	PUSH	DS
	PUSH	OFFSET threadid
	PUSH	SEG cstack_end
	PUSH	OFFSET cstack_end
	CALL	DosCreateThread ;Create timer thread
;
	POP	DS
ASSUME	DS:data
ENDIF
	XOR	AX,AX			;Return 0: No error
	RET
;
;
sdiscard:
;
;------ Discard signal: Stop timer thread (OS/2).
;
	PUSH	DS
	MOV	AX,SEG mdata
	MOV	DS,AX
ASSUME	DS:mdata
IFDEF	protect
;
;------ OS/2:
;
	PUSH	DS
	PUSH	OFFSET tsem3
	CALL	DosSemSet
	PUSH	DS
	PUSH	OFFSET tsem2		;Clear semaphore,
	CALL	DosSemClear		; thread will kill itself!
	PUSH	DS
	PUSH	OFFSET tsem3
	PUSH	-1
	PUSH	-1
	CALL	DosSemWait		;Wait for thread to accept killing
	PUSH	tsem1.highw
	PUSH	tsem1.loww
	CALL	DosSemClear		;Must be cleared BEFORE
	PUSH	tsem1.highw		; closing semaphore !!!
	PUSH	tsem1.loww
	CALL	DosCloseSem		;Close timer tick semaphore
ENDIF
	POP	DS
ASSUME	DS:data
	XOR	AX,AX			;Return 0: no error
	RET
;
sunlink:
;
;------ The timer is running, stop it!
;	Link out of timer interrupt (DOS).
;
	PUSH	DS
	MOV	AX,SEG mdata
	MOV	DS,AX
ASSUME	DS:mdata
	CALL	stop_timer
	MOV	AX,-1
	MOV	tinterval.loww,AX	;tinterval:=-1
	MOV	tinterval.highw,AX
IFNDEF	protect
;
;------ DOS: Set int 1C vector back to original.
;
	LDS	DX,CS:old_1c
	MOV	AX,251CH
	INT	21H
ENDIF
	POP	DS
ASSUME	DS:data
	XOR	AX,AX
	RET				;Return 0: No error
;
signals ENDP
;
;
;	Subroutine clear_event_table
;	============================
;
;	Set the priority field of all event buffers. This means,
;	that they all are free to use.
;
clear_event_table PROC NEAR
	PUSH	DS
	MOV	AX,SEG mdata
	MOV	DS,AX
ASSUME	DS:mdata
;
;------ Set all queue elements to zero priority, signalling
;	free elements.
;
	MOV	CX,queue_limit
	MOV	DI,OFFSET event_buf
	XOR	AX,AX
	@entercrit
	MOV	event_max,CX		;event_max:=queue_limit
ilp1:	MOV	DS:[DI].priority,AX
	ADD	DI,SIZE event_entry
	LOOP	ilp1
;
	MOV	event_missed,AX 	;event_missed:=0
IFNDEF	protect
	MOV	tcounter.loww,AX	;tcounter:=0
	MOV	tcounter.highw,AX
	DEC	AX
ELSE
	DEC	AX
	MOV	thandle,AX		;thandle:=-1
ENDIF
	MOV	tinterval.loww,AX	;tinterval:=-1
	MOV	tinterval.highw,AX
	@exitcrit
	POP	DS
ASSUME	DS:data
	RET
clear_event_table ENDP
;
;
;  Initialisation of unitimer package
;
	PUBLIC	M_unitimer
M_unitimer PROC FAR
;
;------ Stop timer if already running.
;
	CALL	stop_timer
;
;------ Initialize: clear event buffers.
;
	CALL	clear_event_table
;
	RET
M_unitimer ENDP
;
;
;	Subroutine stop_timer
;	=====================
;
;	Stop timer.
;
stop_timer PROC NEAR
IFDEF	protect
	PUSH	DS
	MOV	AX,SEG mdata
	MOV	DS,AX
ASSUME	DS:mdata
	@entercrit
	MOV	AX,thandle
	TEST	AX,AX
	JS	notstart1
	PUSH	AX
	CALL	DosTimerStop
	MOV	thandle,-1
notstart1:
	@exitcrit
	POP	DS
ASSUME	DS:data
ENDIF
	RET
stop_timer ENDP
;
;
;	Subroutine start_timer
;	======================
;
;	First, stop timer if it is running. Start timer with new
;	period. Don't need this for DOS, as periods can be changed
;	dynamically.
;
; Exit: AX:	Error code
;
start_timer PROC NEAR
	XOR	AX,AX
IFDEF	protect
	PUSH	DS
	MOV	DX,SEG mdata
	MOV	DS,DX
ASSUME	DS:mdata
	MOV	DX,tinterval.highw
	CMP	DX,AX
	JL	notimer 		;J/ when no timer interval
	PUSH	DX
	PUSH	tinterval.loww
	PUSH	tsem1.highw
	PUSH	tsem1.loww
	PUSH	DS
	PUSH	OFFSET thandle
	CALL	DosTimerStart
notimer:
	POP	DS
ASSUME	DS:data
ENDIF
	RET
start_timer ENDP
;
;
;====== Timer thread for OS/2 mode, new INT 1C for DOS mode:
;
tthread PROC	FAR
IFDEF	protect
;
;------ OS/2:
;
	MOV	AX,SEG mdata
	MOV	DS,AX
	MOV	ES,AX
ASSUME	DS:mdata,ES:mdata
;
;------ Initially set timer tick semaphore (s1).
;
	PUSH	tsem1.highw
	PUSH	tsem1.loww
	CALL	DosSemSet		;s1 is initially set
;
;------ Loop; wait until 1st or 2nd semaphore clears.
;
tlp1:
	PUSH	DS
	PUSH	OFFSET semindex
	PUSH	DS
	PUSH	OFFSET semlist
	PUSH	-1
	PUSH	-1
	CALL	DosMuxSemWait
	MOV	AX,semindex
	CMP	AX,0
	JE	timersem
	CMP	AX,1
	JNE	ever
;
;------ Stop this thread execution.
;
stopsem:
	PUSH	DS
	PUSH	OFFSET tsem3
	CALL	DosSemClear
	PUSH	0		;end current thread
	PUSH	0		;status code
	CALL	DosExit
;
ever:	JMP	ever		;Loop 'till death
;
;------ A timer tick semaphore was cleared.
;
timersem:
	PUSH	tsem1.highw
	PUSH	tsem1.loww
	CALL	DosSemSet	;Reset the timer tick semaphore
;
ELSE
;
;------ DOS:
;
	STI
	PUSH	DS
	PUSH	ES
	PUSH	AX
;
	MOV	AX,SEG mdata
	MOV	DS,AX
	MOV	ES,AX
ASSUME	DS:mdata, ES:mdata
	CMP	tinterval.highw,0
	JGE	dotimer 		;J/ when timer active
;
;------ No timer active:
;
tquit:	POP	AX
	POP	ES
	POP	DS
	JMP	CS:old_1c		;Jump to old
;
;
;------ The timer is active.
;
dotimer:
	SUB	tcounter.loww,1
	SBB	tcounter.highw,0
	JGE	tquit			;J/ when (tcounter-1)>=0
;
;------ Timer event has occured; issue event.
;
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	BP
	PUSH	SI
	PUSH	DI
	MOV	AX,tinterval.loww	;Set timer counter back
	MOV	tcounter.loww,AX
	MOV	AX,tinterval.highw
	MOV	tcounter.highw,AX
;
ENDIF
;
;------ Search for free event buffer:
;	An event buffer with priority=0 is free.
;
	MOV	DI,OFFSET event_buf
	MOV	CX,event_max
	JCXZ	bad_event		;J/ when no queue elements
	@entercrit
ch_nxt_buf:
	CMP	[DI].priority,0
	JE	not_used		;J/ when found
	ADD	DI,SIZE event_entry
	LOOP	ch_nxt_buf
;
;	No free event buffer found: Too many events:
;
	@exitcrit
bad_event:
	MOV	event_missed,1		;An event has been missed.
;	JMP	bad_event		;ever and ever
	JMP	nextevent
;
;------ Link free event into COMAL's event queue.
;
not_used:
	MOV	AX,event_prio		;Use current priority
	MOV	[DI].priority,AX
	@exitcrit
	PUSH	DI
	ADD	DI,OFFSET proc_handle	;Copy current proc_handle
	MOV	SI,OFFSET timer_proc
	CLD
	MOVSW
	MOVSW
	MOVSW
	MOVSW
	POP	DI
;
;	Link event given by ES:DI into COMAL's event queue.
;
	CALL	link_event		;AX:=link_event(ES:DI)
	TEST	AX,AX
	JNZ	bad_event		;J/ when bad event
nextevent:
IFDEF	protect
	JMP	tlp1
ELSE
	POP	DI
	POP	SI
	POP	BP
	POP	DX
	POP	CX
	POP	BX
	JMP	tquit
ENDIF
;
ASSUME	DS:data, ES:data
tthread ENDP
;
;
;	Implementation of procedure & function bodies
;	=============================================
;
;
;	PROC set_timer_action(PROC action, priority#)
;	---------------------------------------------
;
;	This procedure defines the procedure to be called as a result of a
;	timer event. PROC action must be the name of a CLOSED procedure
;	without arguments, and priority# an integer in the range 1 to 15.
;
	PUBLIC	P_set_timer_action
P_set_timer_action PROC FAR
	PUSH	ES
	MOV	AX,SEG mdata
	MOV	ES,AX
ASSUME	ES:mdata
;
;------ Test priority.
;
	CMP	[DI].set_timer_action_priorityI.highw,0
	JNE	arg_err
	MOV	AX,[DI].set_timer_action_priorityI.loww
	TEST	AX,AX
	JZ	arg_err 		;0<priority<=15
	CMP	AX,15
	JA	arg_err
;
	LEA	SI,[DI].set_timer_action_action
	MOV	DI,OFFSET timer_proc
	@entercrit
	MOV	event_prio,AX		;Set new event_prio
	MOVSW				;Set new timer_proc
	MOVSW
	MOVSW
	MOVSW
	@exitcrit
	POP	ES
ASSUME	ES:data
	RET
;
arg_err:
	MOV	AX,1			;"Argument Error"
	JMP	runerr
P_set_timer_action ENDP
;
;
;	PROC set_timer_speed(interval)
;	==============================
;
;	Sets the interval between timer events to interval seconds. Note,
;	the resolution of the interval is defined by the clock rate, 18.2 Hz.
;	Setting interval to zero stops the generation of events.
;
;	Interval is floating point, in seconds.
;
	PUBLIC	P_set_timer_speed
P_set_timer_speed PROC FAR
	CALL	stop_timer
	LEA	SI,[DI].set_timer_speed_intervalR
	PUSH	DI
	PUSH	ES
	PUSH	DS
	MOV	AX,SEG mdata
	MOV	ES,AX
ASSUME	ES:mdata
;
;	Multiply by 1000.0 to get interval in millisecs in OS/2
;	or in DOS determine the no. of 18.2 Hz ticks.
;
;	OS/2:	tinterval := ROUND(1000.0 * interval)
;	DOS:	tinterval := ROUND(18.2064819335938 * interval)
;
;	If interval<=0 then tinterval:=-1 // Timer inactive
;
	MOV	DI,OFFSET temp
	MOVSW
	MOVSW
	MOVSW
	MOVSW
	MOV	DS,AX
ASSUME	DS:mdata
	MOV	DI,OFFSET temp
IFDEF	protect
	MOV	SI,OFFSET c1000
ELSE
	MOV	SI,OFFSET ctick
ENDIF
	MOV	AX,[DI+6]
	SHL	AX,1
	JBE	stop_it 		;J/ stop timer when interval<=0
;
	CALL	fpmul
	MOV	SI,OFFSET temp
	CALL	fpint
	POP	DS
;
ASSUME	DS:data
	CMP	DS:numerr,0
	JNE	num_err 		;J/ when numeric error
	@entercrit
	MOV	tinterval.loww,AX
	MOV	tinterval.highw,DX
IFNDEF	protect
	MOV	tcounter.loww,AX
	MOV	tcounter.highw,DX
ENDIF
	@exitcrit
	POP	ES
	POP	DI
ASSUME	ES:data
	CALL	start_timer
	TEST	AX,AX
	JNZ	no_timer_start
	RET
;
;------ If error reported from operational system, report further to Comal.
;
no_timer_start:
	JMP	filerr
;
;------ Stop timer
;
ASSUME	DS:mdata, ES:mdata
stop_it:MOV	AX,-1
	@entercrit
	MOV	tinterval.loww,AX	;tinterval:=-1 // inactive
	MOV	tinterval.highw,AX
	@exitcrit
	POP	DS
	POP	ES
ASSUME	DS:data, ES:data
	POP	DI
	RET
;
;
ASSUME	DS:data
num_err:MOV	AL,DS:numerr		;Issue numeric error
	JMP	runerr
P_set_timer_speed ENDP
;
;
;	PROC timer_action_handled
;	=========================
;
;	This procedure MUST be called inside the event handling
;	procedure. It releases the timer queue element in the timer
;	module. If this call is left out, the timer queue will be full
;	after 10 events.
;
;	The variable act_event points to the currently active
;	event_entry buffer.
;
;	**** This procedure is no longer needed because ****
;	**** event entries are released automatically	****
;	**** by COMAL.					****
;
	PUBLIC	P_timer_action_handled
P_timer_action_handled PROC FAR
	RET
P_timer_action_handled ENDP
;
;
;	PROC timer_queue(length#)
;	=========================
;
;	Set length of timer event queue.
;
;	0<=length#<=queue_limit
;
	PUBLIC	P_timer_queue
P_timer_queue PROC FAR
	MOV	AX,[DI].timer_queue_lengthI.loww
	MOV	DX,[DI].timer_queue_lengthI.highw
	TEST	DX,DX
	JNZ	val_err 		;J/ when length >= 65536
	CMP	AX,queue_limit
	JA	val_err 		;J/ when length > queue_limit
	MOV	CX,SEG mdata
	PUSH	DS
	MOV	DS,CX
ASSUME	DS:mdata
	MOV	event_max,AX		;event_max := length
	POP	DS
ASSUME	DS:data
	RET
;
val_err:MOV	AL,5			;"Value out of range"
	JMP	runerr
P_timer_queue ENDP
;
code	ENDS
	END
