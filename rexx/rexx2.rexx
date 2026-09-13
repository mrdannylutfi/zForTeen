/* REXX - Display Matrix output in an ISPF Dialog Box */
address tso "SUBMIT 'USER.JCLLIB(MATSIMUX)'" /* Run calculation engine */

/* Map the populated stem variables to unique panel fields */
M11 = format(matrix.C.1.1, 4, 4); M12 = format(matrix.C.1.2, 4, 4); M13 = format(matrix.C.1.3, 4, 4)
M21 = format(matrix.C.2.1, 4, 4); M22 = format(matrix.C.2.2, 4, 4); M23 = format(matrix.C.2.3, 4, 4)
M31 = format(matrix.C.3.1, 4, 4); M32 = format(matrix.C.3.2, 4, 4); M33 = format(matrix.C.3.3, 4, 4)

/* Invoke the ISPF Display Service to show the window */
address ISPEXEC "DISPLAY PANEL(MATPANEL)"
exit 0
