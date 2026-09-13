/* REXX - Complete ISPF Matrix Panel Pipeline Driver */
signal on novalue

/* Step 1: Dynamically allocate the panel library to ISPPLIB */
address TSO "ALLOC FI(NEWPLIB) DSN('USER.ISPF.PANELS') SHR REUSE"
address ISPEXEC "LIBDEF ISPPLIB LIBRARY ID(NEWPLIB) STACK"

/* Step 2: Invoke the generated C execution script output */
/* This parses 'USER.REXX.LIB(MATOUT)' and populates matrix.C.i.j */
call MATOUT "SILENT" 

/* Step 3: Format and map variables into ISPF Pool Names */
M11 = strip(format(matrix.C.1.1, 4, 4))
M12 = strip(format(matrix.C.1.2, 4, 4))
M13 = strip(format(matrix.C.1.3, 4, 4))
M21 = strip(format(matrix.C.2.1, 4, 4))
M22 = strip(format(matrix.C.2.2, 4, 4))
M23 = strip(format(matrix.C.2.3, 4, 4))
M31 = strip(format(matrix.C.3.1, 4, 4))
M32 = strip(format(matrix.C.3.2, 4, 4))
M33 = strip(format(matrix.C.3.3, 4, 4))

/* Step 4: Display the populated interactive panel */
address ISPEXEC "DISPLAY PANEL(MATPANEL)"

/* Step 5: Clean up library allocations */
address ISPEXEC "LIBDEF ISPPLIB"
address TSO "FREE FI(NEWPLIB)"
exit 0

novalue: 
  say 'Pipeline Error: Attempted to use uninitialized variable on line' sigl
  exit 12
