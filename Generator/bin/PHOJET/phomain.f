      SUBROUTINE PHOMAIN
C**********************************************************************
C
C     example how to call PHOJET using input cards
C
C**********************************************************************
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      SAVE

C  *********** hp compiler settings ******************************
*     ON DOUBLE PRECISION UNDERFLOW IGNORE
*     ON REAL UNDERFLOW IGNORE
C  **************************************************************

      LINP = 5
      CALL PHO_INIT(LINP,IREJ)
      IF(IREJ.NE.0) WRITE(6,'(/1X,A,I8)') 'Error in PHO_INIT',IREJ

      END


      SUBROUTINE PHO_PHIST(IMODE,WEIGHT)
C**********************************************************************
C
C     example histrograms using PHOJET /POHEP1/ and /POHEP2/
C
C     input:  IMODE      -1  initialization
C                        -2  output of results
C                         1  take event statistics
C             WEIGHT     weight of event (if necessary)
C                        for final output: corresponding cross section
C
C**********************************************************************
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      SAVE

      PARAMETER (ZERO   =  0.D0,
     &           IZERO  =  0,
     &           ONE    =  1.D0,
     &           TWO    =  2.D0,
     &           OHALF  =  0.5D0,
     &           PI     =  3.14159265359D0,
     &           TWOPI  =  6.28318530718D0,
     &           TINY   =  1.D-10,
     &           DEPS   =  1.D-10)

C  event debugging information
      INTEGER NMAXD
      PARAMETER (NMAXD=100)
      INTEGER IDEB,KSPOM,KHPOM,KSREG,KHDIR,KACCEP,KSTRG,KHTRG,KSLOO,
     &        KHLOO,KSDPO,KHDPO,KEVENT,KSOFT,KHARD
      COMMON /PODEBG/ IDEB(NMAXD),KSPOM,KHPOM,KSREG,KHDIR,KACCEP,KSTRG,
     &                KHTRG,KSLOO,KHLOO,KSDPO,KHDPO,KEVENT,KSOFT,KHARD

C  global event kinematics and particle IDs
      INTEGER IFPAP,IFPAB
      DOUBLE PRECISION ECM,PCM,PMASS,PVIRT
      COMMON /POGCMS/ ECM,PCM,PMASS(2),PVIRT(2),IFPAP(2),IFPAB(2)

C  general process information
      INTEGER IPROCE,IDNODF,IDIFR1,IDIFR2,IDDPOM,IPRON
      COMMON /POPRCS/ IPROCE,IDNODF,IDIFR1,IDIFR2,IDDPOM,IPRON(15,4)

C  standard particle data interface
      INTEGER NMXHEP

      PARAMETER (NMXHEP=2000)

      INTEGER NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      DOUBLE PRECISION PHEP,VHEP
      COMMON /POEVT1/ NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &                JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),
     &                VHEP(4,NMXHEP)
C  extension to standard particle data interface (PHOJET specific)
      INTEGER IMPART,IPHIST,ICOLOR
      COMMON /POEVT2/ IMPART(NMXHEP),IPHIST(2,NMXHEP),ICOLOR(2,NMXHEP)

      CHARACTER*72 TITLE
      DIMENSION XLIMB(40)

C
C  extract statistics
C
      IF(IMODE.EQ.1) THEN

        CALL PYLIST(1)
C  main particle loop
        DO 100 I=1,NHEP
C  final particle
          IF(ISTHEP(I).EQ.1) THEN
C  particle charge
            ICHAR = IPHO_CHR3(I,2)/3
            PT2 = PHEP(1,I)**2+PHEP(2,I)**2
            IF(PT2.LT.1.D-8) GOTO 99
            PABS = SQRT(PT2+PHEP(3,I)**2)
            IF(PABS.LT.1.D-8) GOTO 99
            PT = SQRT(PT2)
            IF(ABS(PHEP(3,I)).GE.PABS) GOTO 99
            THETA = PHEP(3,I)/PABS
            IF(ABS(THETA).GE.1.D0) GOTO 99
            THETA = ACOS(THETA)
            IF((THETA.LT.1.D-8)
     &        .OR.(PI-THETA.LT.1.D-8)) GOTO 99
C  pseudorapidity
            ETA = -LOG(TAN(THETA/TWO))
C  rapidity
            YY = -1.D+10
            PPLUS  = PHEP(4,I)+PHEP(3,I)
            PMINUS = PHEP(4,I)-PHEP(3,I)
            IF((PPLUS*PMINUS).GT.DEPS) YY = 0.5D0*LOG(PPLUS/PMINUS)
C  transverse energy
            ET = PHEP(4,I)*SIN(THETA)

C  histogramming ....

          ENDIF

 99       CONTINUE
 100    CONTINUE

C
C  initialization
C
      ELSE IF(IMODE.EQ.-1) THEN
C
C  final output
C
      ELSE IF(IMODE.EQ.-2) THEN

C  weight is the cross section of the generated events
      SIGMA = WEIGHT  

      ENDIF

      END

      SUBROUTINE PHO_LHIST(IMODE,WEIGHT)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      END
