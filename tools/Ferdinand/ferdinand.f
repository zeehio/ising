      PROGRAM FERDINAND
C Copyright, Bernd Berg, Oct 10, 2000.
C Ising Model on finite lattice after
C FERDINAND and FISHER, PR 185 (1969) 832.
      include '../ForLib/implicit.sta'
      include '../ForLib/constants.par'
      PARAMETER (BETAINICIAL=2.0)
      PARAMETER (DBETA=0.5,NBETA=2)
      CHARACTER*32 arg
      INTEGER MYSIZE
      WRITE (*,*) '#      L        T            <E>/N'
      DO ii=1,IARGC(),2
         CALL getarg(ii,arg)
         READ (arg,*) MYSIZE
         CALL getarg(ii+1,arg)
         READ (arg,*) BETA
         BETA=ONE/BETA
         CALL MAIN(MYSIZE,BETA)
      END DO
      STOP
      END
      
      SUBROUTINE MAIN(NL,BETA)
      include '../ForLib/implicit.sta'
      include '../ForLib/constants.par'
      PARAMETER (IUO=6,IUD=10)
      INTEGER ML,NS,NLL
      ALLOCATABLE GM(:),GMPR(:), GM2PR(:)
      ML=NL
      NS=ML*NL
      NLL=2*NL-1
      ALLOCATE (GM(0:NLL))
      ALLOCATE (GMPR(0:NLL))
      ALLOCATE (GM2PR(0:NLL))
      CALL GFF(NL,NLL,BETA,GM,GMPR,GM2PR)
      CALL PARTFN(NL,ML,NLL,BETA,GM,GMPR,GM2PR, F,UENG,S,CHT,ZL)
      ACT=(TWO-UENG)/FOUR
      WRITE(*,'(I8,2F16.8)') NL,ONE/BETA,UENG
      DEALLOCATE(GM)
      DEALLOCATE(GMPR)
      DEALLOCATE(GM2PR)
      END
      
      SUBROUTINE GFF(NL,NLL,BETA,GM,GMPR,GM2PR)
      include '../ForLib/implicit.sta'
      include '../ForLib/constants.par'
      DIMENSION C(NLL),GM(0:NLL),GMPR(0:NLL),GM2PR(0:NLL)
      BK=TWO*BETA 
      GM(0)=BK+LOG(TANH(BETA))
      GMPR(0)=TWO+ONE/TANH(BETA)-TANH(BETA)
      SQTNH=(TANH(BETA))**2
      GM2PR(0)=SQTNH-(ONE/SQTNH)
C
      DO ILL=1,NLL
        C(ILL)=COSH(BK)*(ONE/TANH(BK))-COS(PI*ILL/(NL*ONE))
        CSQ=SQRT(C(ILL)*C(ILL)-ONE)
        GM(ILL)=LOG(C(ILL)+CSQ) 
C
        TAPRN=ONE-ONE/(TANH(BK))**2
        CPR=TWO*SINH(BK)/TANH(BK)+TWO*COSH(BK)*TAPRN
        GMPR(ILL)=CPR/CSQ
C
        C2PR1=SINH(BK)-(COSH(BK)/TANH(BK))
        C2PR1=C2PR1*TAPRN*TWO
        C2PR=C2PR1+(COSH(BK)/TANH(BK))
        C2PR=C2PR*FOUR
        GM2PR(ILL)=(C2PR-GMPR(ILL)*GMPR(ILL)*C(ILL))/CSQ
      END DO 
C      
      RETURN
      END

      

      SUBROUTINE PARTFN(NL,ML,NLL,BETA,GM,GMPR,GM2PR, F,UENG,S,CHT,ZL)
      include '../ForLib/implicit.sta'
      include '../ForLib/constants.par'
      DIMENSION GM(0:NLL),GMPR(0:NLL),GM2PR(0:NLL)
      BK=TWO*BETA
      XML=ML*ONE
      XNL=NL*ONE
      NZ=NL-ONE 
C      
C   WE PUT IN A MULTIPLICATION FACTOR Z0 TO PREVENT THE PARTITION
C   FUNCTION BLOW UP.
C
      ZZ1=ZERO
      ZZP1=ZERO
      ZZPP1=ZERO
      ZZ2=ZERO
      ZZP2=ZERO
      ZZPP2=ZERO
      ZZ3=ZERO
      ZZP3=ZERO
      ZZPP3=ZERO
      ZZ4=ZERO
      ZZP4=ZERO
      ZZPP4=ZERO
      Z4Sign=ONE
      ZP1Sign=ONE
      ZP2Sign=ONE
C                                                 
      DO I=0,NZ
        IPI=I+I
        IPIP1=I+I+1
        ZZ1=ZZ1+GM(IPIP1)*XML/TWO+LOG(ONE+EXP(-GM(IPIP1)*XML))
        ZZP1=ZZP1+GMPR(IPIP1)*XML/TWO*
     &     (ONE-EXP(-GM(IPIP1)*XML))/
     &     (ONE+EXP(-GM(IPIP1)*XML))
        ZZPP1=ZZPP1+GM2PR(IPIP1)*XML/TWO*
     &     (ONE-EXP(-GM(IPIP1)*XML))/
     &     (ONE+EXP(-GM(IPIP1)*XML))+
     &     (GMPR(IPIP1)*XML/(ONE+EXP(-GM(IPIP1)*XML)))
     &     **2*EXP(-GM(IPIP1)*XML)
C      
        ZZ2=ZZ2+GM(IPIP1)*XML/TWO+LOG(ONE-EXP(-GM(IPIP1)*XML))
        ZZP2=ZZP2+GMPR(IPIP1)*XML/TWO*
     &    (ONE+EXP(-GM(IPIP1)*XML))/
     &    (ONE-EXP(-GM(IPIP1)*XML))
        ZZPP2=ZZPP2+GM2PR(IPIP1)*XML/TWO*
     &    (ONE+EXP(-GM(IPIP1)*XML))/
     &    (ONE-EXP(-GM(IPIP1)*XML))-
     &    (GMPR(IPIP1)*ML/(ONE-EXP(-GM(IPIP1)*XML)))
     &    **2*EXP(-GM(IPIP1)*XML)
C
        IF(GM(IPI).GE.ZERO) THEN
          ZZ3=ZZ3+GM(IPI)*XML/TWO+LOG(ONE+EXP(-GM(IPI)*XML))
          ZZP3=ZZP3+GMPR(IPI)*XML/TWO*
     &     (ONE-EXP(-GM(IPI)*XML))/(ONE+EXP(-GM(IPI)*XML))
          ZZPP3=ZZPP3+GM2PR(IPI)*XML/TWO*
     &     (ONE-EXP(-GM(IPI)*XML))/(ONE+EXP(-GM(IPI)*XML))+
     &     (GMPR(IPI)*XML/(ONE+EXP(-GM(IPI)*XML)))
     &     **2*EXP(-GM(IPI)*XML)
        ELSE 
          ZZ3=ZZ3-GM(IPI)*XML/TWO+LOG(ONE+EXP(GM(IPI)*XML))
          ZZP3=ZZP3-GMPR(IPI)*XML/TWO*
     &     (ONE-EXP(GM(IPI)*XML))/(ONE+EXP(GM(IPI)*XML))
          ZZPP3=ZZPP3-GM2PR(IPI)*XML/TWO*
     &     (ONE-EXP(GM(IPI)*XML))/(ONE+EXP(GM(IPI)*XML))+
     &     (GMPR(IPI)*XML/(ONE+EXP(GM(IPI)*XML)))
     &     **2*EXP(GM(IPI)*XML)
        END IF
C
        IF(GM(IPI).GE.ZERO) THEN
          ZZ4=ZZ4+GM(IPI)*ML/TWO+LOG(ONE-EXP(-GM(IPI)*XML))
          ZZP4=ZZP4+GMPR(IPI)*XML/TWO*(ONE+EXP(-GM(IPI)*XML))/
     &     (ONE-EXP(-GM(IPI)*XML))
          ZZPP4=ZZPP4+GM2PR(IPI)*XML/TWO*
     &     (ONE+EXP(-GM(IPI)*XML))/(ONE-EXP(-GM(IPI)*XML))-
     &     (GMPR(IPI)*XML/(ONE-EXP(-GM(IPI)*XML)))
     &     **2*DEXP(-GM(IPI)*XML)
        ELSE
          ZZ4=ZZ4-GM(IPI)*XML/TWO+LOG(ONE-EXP(GM(IPI)*XML))
          Z4Sign=-Z4Sign
          ZZP4=ZZP4-GMPR(IPI)*XML/TWO*
     &     (ONE+EXP(GM(IPI)*XML))/(ONE-EXP(GM(IPI)*XML))
          ZZPP4=ZZPP4-GM2PR(IPI)*XML/TWO*
     &     (ONE+EXP(GM(IPI)*XML))/(ONE-EXP(GM(IPI)*XML))-
     &     (GMPR(IPI)*XML/(ONE-EXP(GM(IPI)*XML)))
     &     **2*EXP(GM(IPI)*XML)
        END IF
      END DO
C
      ZP1Sign=SIGN(ONE,ZZP1)
      ZP2Sign=SIGN(ONE,ZZP2)
      ZP3Sign=SIGN(ONE,ZZP3)
      ZP4Sign=SIGN(ONE,ZZP4)
      ZL=ZZ1+LOG(ONE+EXP(ZZ2-ZZ1)+EXP(ZZ3-ZZ1)+
     &   Z4Sign*EXP(ZZ4-ZZ1))
      ZLP=ZZP1*EXP(ZZ1-ZL)+ZZP2*EXP(ZZ2-ZL)+
     &    ZZP3*EXP(ZZ3-ZL)+Z4Sign*ZZP4*EXP(ZZ4-ZL)
      UENG=-ONE/TANH(BK)-ZLP/XML/XNL
      ZZPP1=ZZPP1+(ZZP1)**TWO
      ZZPP2=ZZPP2+(ZZP2)**TWO
      ZZPP3=ZZPP3+(ZZP3)**TWO
      ZZPP4=ZZPP4+(ZZP4)**TWO
      CHT1=ZZPP1*EXP(ZZ1-ZL)+ZZPP2*EXP(ZZ2-ZL)+
     &     ZZPP3*EXP(ZZ3-ZL)+Z4Sign*ZZPP4*EXP(ZZ4-ZL)
      CHT=(CHT1-ZLP**TWO)*(BK*BK)/(FOUR*XNL*XML)-
     &     (BK/SINH(BK))**TWO/TWO
      ZL=ZL+XML*XNL/TWO*LOG(TWO*SINH(BK))-LOG(TWO) 
C
      F=-ZL/(XML*XNL)/BETA
      S=(UENG-F)*BETA
C
      return
      end

