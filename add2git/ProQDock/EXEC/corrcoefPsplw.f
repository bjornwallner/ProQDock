      program lineskip

!     Pearson's product moment correlation coeficient for a pair of potential
!     realized on the same set of surface points due to different set of atoms

      character(80)::pot1f,pot2f
      character(13)::atomdesc1,atomdesc2
      real::x(10000),y(10000),xmc(10000),ymc(10000),
     &xsc(10000),ysc(10000)
      integer::isc,imc

      call getarg(1,pot1f)
      call getarg(2,pot2f)

      open (unit=1,file=pot1f,status='old')
      open (unit=2,file=pot2f,status='old')

      ih = 0


      sumpot1 = 0.0
      sumpot2 = 0.0
      sumx = 0.0
      sumy = 0.0
      sumxs = 0.0
      sumys = 0.0
      sumxm = 0.0
      sumym = 0.0
      ic = 0
      isc = 0 
      imc = 0

18         read(1,34,end=20)atomdesc1,x1,y1,z1,pot1
!           write(*,34)atomdesc1,x1,y1,z1,pot1
           read(2,34,end=20)atomdesc2,x2,y2,z2,pot2
!           write(*,34)atomdesc2,x2,y2,z2,pot2
                if (atomdesc1 .eq. atomdesc2 .and.
     &          x1 == x2 .and.
     &          y1 == y2 .and.
     &          z1 == z2)then
                ic = ic + 1
                x(ic) = pot1
                y(ic) = pot2
                sumx = sumx + x(ic)
                sumy = sumy + y(ic)
                   if (atomdesc1(1:3).ne.'N  '.and.
     &atomdesc1(1:3).ne.'CA '.and.atomdesc1(1:3).ne.'C  '.and.
     &atomdesc1(1:3).ne.'O  '.and.atomdesc1(1:3).ne.'H  '.and.
     &atomdesc1(1:3).ne.'HA '.and.atomdesc1(1:3).ne.'OXT')then
                   isc = isc + 1
                   xsc(isc) = pot1
                   ysc(isc) = pot2   
                   sumxs = sumxs + xsc(isc)
                   sumys = sumys + ysc(isc)
                   else
                   imc = imc + 1
                   xmc(imc) = pot1
                   ymc(imc) = pot2   
                   sumxm = sumxm + xmc(imc)
                   sumym = sumym + ymc(imc)
                   endif
            endif

      goto 18

20    continue

30    continue
40    continue

34    format(a13,7x,3f10.4,10x,f10.4)

      xmean = sumx/float(ic)
      ymean = sumy/float(ic)

      xscm = sumxs/float(isc)
      yscm = sumys/float(isc)

      xmcm = sumxm/float(imc)
      ymcm = sumym/float(imc)

!      print*,sumx,sumy,ic,sumxs,sumys,isc,sumxm,sumym,imc

!      print*,xmean,ymean,ic,xscm,yscm,isc,xmcm,ymcm,imc

!===============================================================
!     Calculate Pearson product moment correlation coefficient
!     CCP(x,y) = CoV(x,y)/(sig(x)*sig(y))
!===============================================================

      sumdxy = 0.0
      sumsqx = 0.0
      sumsqy = 0.0

      sumdxyS = 0.0
      sumsqxS = 0.0
      sumsqyS = 0.0

      sumdxyM = 0.0
      sumsqxM = 0.0
      sumsqyM = 0.0

      do i = 1,ic
      write(777,812)x(i),y(i)
      sumdxy = sumdxy + ((x(i)-xmean)*(y(i)-ymean))
      sumsqx = sumsqx + (x(i)-xmean)**2
      sumsqy = sumsqy + (y(i)-ymean)**2
      enddo

      do i = 1,isc
      write(888,812)xsc(i),ysc(i)
      sumdxyS = sumdxyS + ((xsc(i)-xscm)*(ysc(i)-yscm))
      sumsqxS = sumsqxS + (xsc(i)-xscm)**2
      sumsqyS = sumsqyS + (ysc(i)-yscm)**2
      enddo
 
      do i = 1,imc
      write(999,812)xmc(i),ymc(i)
      sumdxyM = sumdxyM + ((xmc(i)-xmcm)*(ymc(i)-ymcm))
      sumsqxM = sumsqxM + (xmc(i)-xmcm)**2
      sumsqyM = sumsqyM + (ymc(i)-ymcm)**2
      enddo

812   format(2f8.3)

      sigx = sqrt(sumsqx/float(ic))
      sigy = sqrt(sumsqy/float(ic))
      covxy = sumdxy/float(ic)

      sigxS = sqrt(sumsqxS/float(isc))
      sigyS = sqrt(sumsqyS/float(isc))
      covxyS = sumdxyS/float(isc)

      sigxM = sqrt(sumsqxM/float(imc))
      sigyM = sqrt(sumsqyM/float(imc))
      covxyM = sumdxyM/float(imc)

      corrcoef = covxy/(sigx*sigy)
      Elcomp = -corrcoef

      corrcoefS = covxyS/(sigxS*sigyS)
      ElcompS = -corrcoefS

      corrcoefM = covxyM/(sigxM*sigyM)
      ElcompM = -corrcoefM

!      print*,ic,isc,imc,Elcomp,ElcompS,ElcompM

!      write(*,78)Elcomp,ElcompS,ElcompM
      write(*,78)Elcomp
78    format(f8.3)

      endprogram lineskip
