PAGE	66,132
;
;------ Symbols for use by assembler coded modules for COMAL.
;	UniComal Version 3.10C
;
;------ Routines supplied by COMAL.
;
EXTRN	runerr:FAR
EXTRN	filerr:FAR
EXTRN	report:FAR
EXTRN	get_err_msg:FAR
EXTRN	get_err_num:FAR
EXTRN	get_file:FAR
EXTRN	set_file:FAR
EXTRN	fpadd:FAR
EXTRN	fpsub:FAR
EXTRN	fpsubr:FAR
EXTRN	fpmul:FAR
EXTRN	fpdiv:FAR
EXTRN	fpdivr:FAR
EXTRN	fpint:FAR
EXTRN	intfp:FAR
EXTRN	fpsqr:FAR
EXTRN	fpsin:FAR
EXTRN	fpcos:FAR
EXTRN	fptgn:FAR
EXTRN	fpatn:FAR
EXTRN	fplog:FAR
EXTRN	fpexp:FAR
EXTRN	outchar:FAR
EXTRN	bye:FAR
EXTRN	create_str:FAR
EXTRN	remove_str:FAR
EXTRN	dir_entry:FAR
EXTRN	check_dir:FAR
EXTRN	get_char:FAR
EXTRN	put_char:FAR
EXTRN	editor_functions:FAR
EXTRN	move_from:FAR
EXTRN	move_to:FAR
EXTRN	link_event:FAR
EXTRN	scan_env:FAR
;
;
.SALL				;Suppress macro listing
;
IFDEF	protect
	 .286c
ENDIF
;
intvar	STRUC
loww	  DW ?			;Low part of integer
highw	  DW ?			;High part of integer
intvar	ENDS
;
farptr	STRUC
offw	  DW ?			;Offset of far pointer
segw	  DW ?			;Segment selector of far pointer
farptr	ENDS
;
;
;------ String variable layout:
;
strvar	STRUC
maxlen	  DW ?			;Dimensioned length
curlen	  DW ?			;Current length of string
s	  DB 1 DUP(?)		;Storage for string
strvar	ENDS
;
;
;------ Array dope vector layout:
;
adope	STRUC			;Dope vector for arrays
abase	  DD	?		;Address of first array component
awaste	  DW	?		;No. of bytes wasted before array - for alignment!
aibase	  DW	?		;Element base in first segment (used in OS/2 only)
aisize	  DW	?		;Elements per segment (used in OS/2 only)
asize	  DW	?		;Size of each element (always even)
adim4	  DW	?		;Dimension*4
alow1	  DW	?		;First lowbound(negated) (-32767..32767)
alen1	  DW	?		;Length of index space (0..65535)
alow2	  DW	?		;Second lowbound(negated) (-32767..32767)
alen2	  DW	?		;Length of index space (0..65535)
alow3	  DW	?		;Third lowbound(negated) (-32767..32767)
alen3	  DW	?		;Length of index space (0..65535)
				; etc...
adope	ENDS
;
;
;------ Index space:
;
index	STRUC			;Index space record
ilow	  DW	?		;Lowbound(negated) (-32767..32767)
ilen	  DW	?		;Length of index space (0..65535)
index	ENDS
;
;
;------ Event message buffer:
;
event_entry STRUC		;Event entry record
next_event    DD ?		;Link to next event
priority      DW ?		;Priority of this event
proc_handle   DQ ?		;Descriptor of PROCedure to execute
event_entry ENDS
;
;
;  Open file modes:
;
o_read	EQU	01H		;Opened for reading
o_write EQU	02H		;Opened for writing
;
l_read	EQU	04H		;Deny read
l_write EQU	08H		;Deny write
;
o_ran	EQU	10H		;Opened for random access
;
x_commit EQU	20H		;Commit each WRITE/PRINT
x_app	EQU	40H		;Append to file
x_locks EQU	80H		;Explicit LOCKs is defined
;
trap_esc EQU	4		;flags: TRAP ESC- flag
breaked  EQU	8		;flags: ESC value
;
;compiler EQU	10B		 ;system: 0=Interactive, 1=Compiled
;scr_sw  EQU  1100B		 ;system: 00=/B, 01=/A, 10=/R, 11=/Q
;cur_sw  EQU 10000B		 ;system: 0=/S, 1=/H
;
;
;------ ML signal numbering
;
s_load		EQU	0	;Issued during CHAIN, RUN, SCAN
s_discard	EQU	2	;Issued by NEW, LOAD, CHAIN, BYE(, RUN, SCAN)
s_link		EQU	4	;Issued during RUN, CHAIN
s_unlink	EQU	6	;Issued before SCAN, RUN, CHAIN, NEW, BYE
s_stop		EQU	8	;Issued by STOP, END or CTRL-Break
s_con		EQU    10	;Issued by CON
s_error_1	EQU    12	;Issued by run time error before the message
s_error_2	EQU    14	;Issued by run time error after the message
s_pass_1	EQU    16	;Issued by PASS before call
s_pass_2	EQU    18	;Issued by PASS after call
s_reset_video	EQU    20	;Issued after execution of ALT-O
s_use		EQU    22	;Issued in USE-statement
;
;
;------ COMAL data segment.
;
data	SEGMENT AT 0
ASSUME	CS:data
etop		DW ?	;Top of evaluation stack. (etop<efree)
efree		DW ?	;Max of evaluation stack
numerr		DB ?	;<>0: Numeric error number
flags		DB ?	;Flags
escape		DB ?	;CTRL-Break has been depressed
cstat		DB ?	;Mode of operation
hshift		DW ?	;Number of times to shift carry when indexing arrays
hadd		DW ?	;Value to add to get next segment in arrays (2^hshift)
;
cdata_seg	DW ?	;Segment of cdata
;
retcode 	DD ?			;ret. code from last PASS
txt		DB 100h DUP(?)		;Translated string constant.
temp_bp 	DW ?			;Save for BP during asm.calls
inbuf		DB 100h DUP(?)		;Input buffer.
;
act_event	DD ?			;Handle for actually running event
					;=0 if no event handled at the moment
act_priority	DW ?			;Priority of handled event
;
next_code	DW ?			;Jump mask
xtop		DW ?			;Current top of evaluation stack
emax		DW ?			;Max evaluation stack address
;
first_event	DD ?			;Pointer to first event in event_queue
;
IFDEF	protect
a1max		DD ?
a2max		DD ?
atop		DD ?			;strings from 0 to atop
avar		DD ?			;dims from 0 to avar
htop		DD ?			;real heap
hmax		DD ?
hfree		DD ?			;areas, each area contains a pointer
;					;to the next entry, and the length
;					;of the area, incl. pointer (total 6
;					;bytes).
a1seg		DW ?
a2seg		DW ?
hseg		DW ?
ELSE
amax		DD ?			;Top of Arr & Str stack (FAR)
atop		DD ?			;Strings from aseg to atop (FAR)
aseg		DW ?			;Starting segment of Arr & Str stack
avar		DD ?			;Dims from avar to amax
					;note, WORD PTR atop/amax/avar < 16!
htop		DD ?			;Current top of Heap space (FAR)
hmax		DD ?			;Max top of heap (FAR)
hseg		DW ?			;First segment of heap space.
hfree		DD ?			;Free list of disposed elements
ENDIF
data	ENDS
;
;
;------ COMAL system variables.
;
cdata	SEGMENT AT 0
;
psp_seg  DW ?			;Segment of program prefix
;
; Variables used by the editor
;
col	DB	?		;column number for editor
row	DB	?		;row number for editor
col_min DB	?		;min col no. if in window. must start on word boundary.
col_max DB	?		;max col no. if in window
row_min DB	?		;min row no. if in window
row_max DB	?		;min row no. if in window
row2	DB	?
pagelen DB	?		;no of rows on page.
linelen DB	?		;no of chars on a line.
;
inserting DB	?		;0=replace, 1=insert, +128=insert input
text_attr DB	?		;attribute of text displayed
err_attr DB	?		;attribute of error message
save_attr DB	?		;save of attribute during error printing
indent_attr DB	?		;attribute of right arrow
blank_attr DB	?		;attribute used for blank
disp1_attr DB	?		;attribute for number in display line
disp2_attr DB	?		;attribute for text in display line
graphics DB	?		;0: alpha, 1: graphics
;
actpg	    DB	?		;actual page number.
;
norm_attr   DB	?		;Attribute of normal text
;
scan_attr   DB	0
;
show_on 	DB	5	;1: show display during editing
				;0: don't show
				;2: show display always.
				;4: show top status during editing.
				;- never to be seen during run!
show_status	DB	?	;00: off, 01: bottom, 10: top, 11: both
show_cstat	DB	0	;cstat last time display was generated
;
old_shift	DB	?	;old storage of shift state
;
exit_code_bye DB 0		;Code to return when BYEing
;
coprocessor DB 0
;
fkey_error  DB	?		;Error flagged during function key insertion
;
IFDEF	protect
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
term_pos    DB	0		;Terminating position within input field
input_pos   DB	0		;Start position within input field
inp_term	DB  512/8  DUP(?)	;512 bit terminator table
term		DW	?		;Termination char. for last INPUT
;
par_pnt  DW 0			;Address of parameters
par_len  DW 0			;Length of parameters
bootprog DW 0			;Pointer to first program to run
;
curtype     DB	0		;0:Soft, 1:Hard, 2:Hard always on
cursize     DB	2		;0:No size, 1-8: size
;
char_inst   DW 0		;char lost during inserting
;
;------ Compiler variables ------------------------------------------------
;	The order and amount is used by COMALC.EXE
;
	EVEN
	DB	6 DUP(?)
	DB  32 DUP(0)
psize	DW 0FFFEH	;Specified program segment size/load module size
dsize	DW 0FFFEH	;Specified stack segment size
hsize	DD 0FFFEH	;Specified heap size
ssize	DD 0FFFEH	;Specified array size (combined string&dim)
ksize	DW 00800H	;Specified key size
keyb_offs DB 0		;Keyboard function code offset
			;-1: do Test_keyboard
country DB	'US'    ;COUNTRY="US" // profile variable
;
;	bios_sw:
;	  xx1xxxxx	;Extended keyboard interpretation
;	  xx0xxxxx	;Standard keyboard interpretation
;	  xxx1xxxx	;Bell on
;	  xxx0xxxx	;Bell off
;	  xxxx00xx	;Soft cursor
;	  xxxx01xx	;Hard cursor
;	  xxxx10xx	;Hard cursor always on
;	  xxxxxx00	;Bios
;	  xxxxxx01	;Appropriate
;	  xxxxxx10	;Retrace
;	  xxxxxx11	;Quick
;
IFDEF	protect
bios_sw  DB 10000B	;0=BIOS, 1=Appropriate, 2=Retrace, 3=Quick
ELSE
bios_sw  DB 10001B	;0=BIOS, 1=Appropriate, 3=Quick
ENDIF
;ENDIF
	 DB 0		;0=Look for switches, 1=Don't
;
msg1	DB 500 DUP(?)
msg1_len DW	?
regno	DD	?
userno	DD	?
;
;
;------ End of compiler variables -----------------------------------------
;
cdata	ENDS
;
EXTRN	ml_prog:WORD			;Start of PROG segment
perr_seg EQU	WORD PTR 1AH		;Segment of messages
perr_ofs EQU	WORD PTR 1CH		;Error number offset
perr_max EQU	WORD PTR 1EH		;Last error number + 1

;
;------ Macros to get addresses of small parameters (integers/reals)
;
@val_smal MACRO ofs,nam
	LEA	ofs,[DI+nam]
       ENDM
;
@ref_smal MACRO sgm,ofs,nam
	L&sgm	ofs,[DI+nam]
       ENDM
;
; Macros to get addresses of huge parameters (strings/strucs/arrays)
;
@val_huge MACRO sgm,ofs,nam
	L&sgm	ofs,[DI+nam]
       ENDM
;
@ref_huge MACRO sgm,ofs,nam
	L&sgm	ofs,[DI+nam]
       ENDM
;
;----- Macros for defining standard messages.
;
stdmsg?=0				;Assume no messages

cmsg1	MACRO n,text
IFB	<text>
stdmsg&n EQU THIS BYTE
ELSE
stdmsg&n DB text
ENDIF
	ENDM

cmsg2	MACRO n
	DW stdmsg&n
	ENDM

CSTDMSG MACRO last			;Messages begin
stdmsg?=1				;There are standard messages
stdnmsg=last+1				;Number of standard messages
cstd	SEGMENT PARA PUBLIC 'data'
std_end  DW std_last-std_end		;Offset to end of segment
std_nums DW (std_te-std_tb)/2		;Total number of messages
;
;------- Offsets to messages:
;
std_tb	 EQU THIS WORD
std?=0
	REPT last+1
	cmsg2 %std?
std?=std?+1
	ENDM
;
std_te	 DW std_last			;End of messages
std?=0
	ENDM

CENDSTDMSG MACRO			;Messages end
std_last LABEL BYTE
cstd	ENDS
	ENDM

CMSG	MACRO n, text			;Message
	REPT n-std?
	cmsg1 %std?,<>
std?=std?+1
	ENDM
	cmsg1 %std?,<text>
std?=std?+1
	ENDM

;------ Standard error messages --------------------------------------------

eREPORT 	EQU 0	;REPORT error
eArgument	EQU 1	;Argument error
eOverflow	EQU 2	;Overflow
eDivZero	EQU 3	;Division by zero
eSubString	EQU 4	;Substring error
eRangeErr	EQU 5	;Value out of range
eIllBound	EQU 7	;Illegal bound
eInvFormat	EQU 8	;Invalid format string
eIndexRange	EQU 10	;Index out of range
eInvFile	EQU 11	;Invalid file name
eNoMsg		EQU 26	;No such message number local to this MODULE/STRUC
eNoMsgFile	EQU 28	;No message file for this MODULE/STRUC
eEOF		EQU 201 ;End of file
eAlreadyOpen	EQU 202 ;File already open
eNotOpen	EQU 203 ;File not open
eNotInput	EQU 204 ;Not input file
eNotOutput	EQU 205 ;Not output file
eTooManyFiles	EQU 209 ;Too many files open
eReadErr	EQU 210 ;Read error
eWriteErr	EQU 211 ;Write error
eNotReady	EQU 213 ;Drive not ready
ePrinter	EQU 214 ;Printer out of paper or not ready
eFileNotFound	EQU 216 ;File not found
eUnknownDev	EQU 217 ;Unknown device
eIllOperation	EQU 218 ;Illegal operation
eIOBreak	EQU 219 ;I/O break
ePathNotFound	EQU 220 ;Path not found
eProtected	EQU 221 ;Attempt to write on write-protected diskette
eDiskFull	EQU 224 ;Disk full
eAlreadyExists	EQU 225 ;File already exists
eOutOfRes	EQU 226 ;Out of resources
eNetErr 	EQU 227 ;Network error
eBusy		EQU 228 ;Busy
eAccessDenied	EQU 229 ;Access denied
