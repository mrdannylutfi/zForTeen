//MATESSLX JOB (ACCT),'ESSL PIPELINE RUN',CLASS=A,MSGCLASS=X,
//             NOTIFY=&SYSUUID
//*-------------------------------------------------------------------*
//* STEP 0: PRE-ALLOCATE THE RUNTIME LIBRARIES IF NOT PRESENT
//*-------------------------------------------------------------------*
//ALLOCATE EXEC PGM=IEFBR14
//LOADLIB  DD DSN=USER.LOAD.LIB,
//            DISP=(MOD,CATLG,DELETE),
//            SPACE=(CYL,(1,1,10)),
//            DCB=(RECFM=U,LRECL=0,BLKSIZE=32760)
//REXXLIB  DD DSN=USER.REXX.LIB,
//            DISP=(MOD,CATLG,DELETE),
//            SPACE=(TRK=(5,5,10)),
//            DCB=(RECFM=FB,LRECL=80,BLKSIZE=27920)
//*-------------------------------------------------------------------*
//* STEP 1: COMPILE THE C SOURCE CODE WITH IBM Z14 VECTOR ENHANCEMENTS
//*-------------------------------------------------------------------*
//COMPILE  EXEC EDCC,
//         OUTFILE='USER.LOAD.LIB(MATESSL),DISP=SHR',
//         CPARM='OPT(3),ARCH(12),TUNE(12),LONGNAME,RENT'
//COMPILE.SYSIN DD DISP=SHR,DSN=USER.C.SRC(MATESSL)
//* Ensure the binder includes the standard LE and ESSL side-decks
//BIND.SYSLIB DD DISP=SHR,DSN=CEE.SCEELKED
//            DD DISP=SHR,DSN=ESSL.SESSLKED
//*-------------------------------------------------------------------*
//* STEP 2: EXECUTE MATRIX SIMULATION ENGINE
//*-------------------------------------------------------------------*
//RUNMATH  EXEC PGM=MATESSL,COND=(4,LT)
//STEPLIB  DD DISP=SHR,DSN=USER.LOAD.LIB
//         DD DISP=SHR,DSN=ESSL.SESSLLIB   <-- Active ESSL Call Runtime
//CEEEDUMP DD SYSOUT=*
//SYSPRINT DD SYSOUT=*
//SYSOUT   DD SYSOUT=*
//*-------------------------------------------------------------------*
//* STEP 3: EXECUTE THE GENERATED REXX SCRIPT NATIVELY IN BATCH TSO
//*-------------------------------------------------------------------*
//RUNREXX  EXEC PGM=IKJEFT01,COND=(4,LT)
//SYSTSPRT DD SYSOUT=*
//SYSTSIN  DD *
  ALLOC FIL(SYSPROC) DSN('USER.REXX.LIB') SHR REUSE
  %MATOUT
  FREE FIL(SYSPROC)
/*
//
