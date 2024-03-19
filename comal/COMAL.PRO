// New profile for UniComal V3.11, created 2020-04-03 //
//----- Memory, shown by SIZE
STACKSIZE      32000          // Space for skalar variables and calls
PROGSIZE       65520          // Main program size
SWAPSIZE       0              // Swap area size
ARRAYSIZE      15000          // Space for arrayts and strings (DIM)
HEAPSIZE       5000           // Heap space (CREATE/DISPOSE)
KEYSIZE        2048           // Space for function keys (FKEY)
HELPSIZE       21             // Number of help entries
//----- Colors for CGA/EGA/VGA:  For Monochrome:
TEXTCOLOR      green/              white
ARROWCOLOR     b'blue+i'white/     b'black+i'white
ERRORCOLOR     b'blue+i'white/     b'black+i'white
KEYCOLORS      b'black+white/      b'black+i'white, b'white/underline+black
HELPCOLOR      b'white+blue/       b'white+black
HELPHIGHLIGHT  b'white+red/        b'white+i'white
HELPCURSOR     b'white+yellow/     b'black+i'white
HELPLABEL      b'white+i'white/    b'white+i'white
CURSOR         soft           // Cursor control mode
SHOWKEY        on             // Bottom function key line
SHOWSTATUS     on             // Top status line
SCREEN         auto           // For DOS-Comal only
KEYBOARD       auto           // Keyboard type (standard/extended)
//----- Sound
BELL           on             // Sound by error messages
BELLSOUND      1000, 200      // OS/2-Comal only
//----- Externals
COUNTRY        "DK"           // Use message files ending with "DK"
// Search paths used when loading external modules, structures,
// procedures, and functions. Each path is separated by  .
// The special path, "*", means the directory, where COMAL.EXE
// was found. The standard modules can be found there.
EXTPATH        ".;c:\comal\modules;c:\comal" // Search path
MESSAGE        "c:\comal\comaldk.msg" // COMAL message file
SWAPFILE       "c:\comal\comal.swp" // COMAL swap file name
//----- Editing function keys
// Unshifted F1-F10 //
FKEY         1,0,"HJ’LP",   ""0""9""
FKEY         2,0,"RUNÙ",    ""0""23"RUN"13""
FKEY         3,0,"AUTOÙ",   ""0""23"AUTO"13""
FKEY         4,0,"RENUMÙ",  ""0""23"RENUM"13""
FKEY         5,0,"DIRÙ",    ""0""23"DIR"13""
FKEY         6,0,"DIR CML", ""0""23"DIR ""*.CML"""13""
FKEY         7,0,"CONÙ",    ""0""23"CON"13""
FKEY         8,0,"LOAD """, "LOAD """
FKEY         9,0,"FIND """, "FIND """
FKEY         10,0,"SEL CON","SELECT OUTPUT ""CON:"""13""
// Shift F1-F10 //
FKEY         1,1,"LISTÙ",   ""0""23"LIST"13""
FKEY         2,1,"SCANÙ",   ""0""23"SCAN"13""
FKEY         3,1,"DEL",     "DEL "
FKEY         4,1,"SIZEÙ",   ""0""23"SIZE"13""
FKEY         5,1,"CHDIR ""","CHDIR """
FKEY         6,1,"DIR DIR", ""0""23"DIR ""*."""13""
FKEY         7,1,"ENTER ""","ENTER """
FKEY         8,1,"SAVE """, "SAVE """
FKEY         9,1,"CHANGE""","CHANGE """
FKEY         10,1,"SEL PRN","SELECT OUTPUT ""PRN:"""13""
// Ctrl F1-F10 //
FKEY         1,2,"LIST """, "LIST """
FKEY         2,2,"RUN DIR", ""0"RRUN """9""9""0"M"""0"u"13""
FKEY         3,2,"MERG DI", ""0"RMERGE """9""9""0"M"0"M"0"M"""0"u"13""
FKEY         4,2,"MERGE ""","MERGE """
FKEY         5,2,"MKDIR ""","MKDIR """
FKEY         6,2,"DEL DIR", ""0"RDELETE """9""0"M"0"M"0"M"0"M"""0"u"
FKEY         7,2,"ENTR DI", ""0"RENTER """9""9""0"M"0"M"0"M"""0"u"13""
FKEY         8,2,"LOAD DI", ""0"RLOAD """9""9""0"M"0"M"""0"u"13""
FKEY         9,2,"DELETE""","DELETE """
FKEY         10,2,"SEL INP","SELECT INPUT """
// Alt F1-F10 //
FKEY         1,4,"UNIT A:", ""0""23"UNIT ""A:"""13""
FKEY         2,4,"UNIT B:", ""0""23"UNIT ""B:"""13""
FKEY         3,4,"UNIT C:", ""0""23"UNIT ""C:"""13""
FKEY         4,4,"UNIT D:", ""0""23"UNIT ""D:"""13""
FKEY         5,4,"CD DIR",  ""0"RCHDIR """9""9""0"M"0"M"0"M"""0"u"13""
FKEY         6,4,"DIR LST", ""0""23"DIR ""*.LST"""13""
FKEY         7,4,"",        ""
FKEY         8,4,"",        ""
FKEY         9,4,"SAVEÙ",   ""0""23"SAVE"13""
FKEY         10,4,"SAVE !", ""0""23"SAVE"13""13""
//----- Help files
HELPFILE       "c:\comal\us\help.hlp" // Help entry
HELPFILE       "c:\comal\us\relnotes.hlp" // Help entry
HELPFILE       "c:\comal\us\comalc.hlp" // Help entry
HELPFILE       "c:\comal\us\system.hlp" // Help entry
HELPFILE       "c:\comal\us\graphics.hlp" // Help entry
HELPFILE       "c:\comal\us\sound.hlp" // Help entry
HELPFILE       "c:\comal\us\unimouse.hlp" // Help entry
HELPFILE       "c:\comal\us\unitimer.hlp" // Help entry
HELPFILE       "c:\comal\us\unidump.hlp" // Help entry
HELPFILE       "c:\comal\us\windows.hlp" // Help entry
HELPFILE       "c:\comal\us\menus.hlp" // Help entry
HELPFILE       "c:\comal\us\unihlp.hlp" // Help entry
HELPFILE       "c:\comal\us\scom.hlp" // Help entry
