c     PROGRAM TO CALCULATE POINTS ON THE SURFACE OF A  SPHERE (for isolated metal ions)

      common/surface/xdot(2500),ydot(2500),zdot(2500)
      common/normal/xnor(2500),ynor(2500),znor(2500)
      common/const/pi
      dimension  dtht(5),nobst(5),arcc(5)
      dimension  radin(27)
      character(3)::mres(27),matom(27)
      character(3)::mat(100),mrs(100),matw,mrw
      integer::imr(100)
      real::xm(100),ym(100),zm(100)
      character(4)::atag
      character(3)::atype,rtype

      open(unit=1, file='msph.dot', status='new')
      open(unit=3, file='met.radii',status = 'old')
      open(unit=4, file='inp.pdb',status='old')
c---------------------------------------------------
      pi= acos(-1.00)
      idensity = 10
c--------------------------------------------------

      inm = 0

      do i = 1,50000
      read(4,617,end=10)atag,atype,rtype,irn,xn,yn,zn
          if (atag.eq.'HETA')then
          inm = inm + 1
          mat(inm) = atype
          mrs(inm) = rtype
          imr(inm) = irn
          xm(inm) = xn
          ym(inm) = yn
          zm(inm) = zn
          endif
      enddo

10    continue
617   format(a4,9x,a3,1x,a3,3x,i3,4x,3f8.3)

!      print*,inm


       do   93    i = 1,27
       read(3,97)matom(i),mres(i),radin(i)
97     format(a3,3x,a3,2x,f4.2)
93     continue


      L0: do im = 1,inm

          L1: do jj = 1,27
             if ((mat(im).eq.matom(jj)).and.(mrs(im).eq.mres(jj)))then
             radm = radin(jj)
             imw = imr(im)
             matw = matom(jj)
             mrw = mres(jj)
             xtran = xm(im)
             ytran = ym(im)
             ztran = zm(im)
             goto 536
             endif
          enddo L1

536     continue
          
             if(radm.eq.0)then
             num00 = 0
             goto 51 
             endif

      ncal = ifix(float(idensity)*4.0*pi*radm*radm)

             do 71 i=1,5
             dtht(i)=0
             nobst(i)=0
71           continue    


      do  61 ii = 1, 50000

      dth =  1.00 + float(ii-1) * 0.020
      if(dth.gt.180.0) then
      goto 9
      endif

      Call angcheck(dth,radm,nobs,arc)

      if(nobs.eq.ncal)then
      dtht(1)  = dth
      nobst(1) = nobs
      arcc(1)  = arc 
      endif

      if( (nobs.eq.ncal+1).or.(nobs.eq.ncal-1) )then
      dtht(2)  = dth
      nobst(2) = nobs
      arcc(2) =  arc       
      endif

      if( (nobs.eq.ncal+2).or.(nobs.eq.ncal-2) )then
      dtht(3)  = dth
      nobst(3) = nobs
      arcc(3)  = arc
      endif

      if( (nobs.eq.ncal+3).or.(nobs.eq.ncal-3) )then
      dtht(4)  = dth
      nobst(4) = nobs
      arcc(4)  = arc
      endif

      if( (nobs.eq.ncal+4).or.(nobs.eq.ncal-4) )then
      dtht(5)  = dth
      nobst(5) = nobs
      arcc(5)  = arc  
      endif
61    continue

9     continue   


        do  77  i = 1,5
        if(dtht(i).ne.0)then
39      format(5x,f5.2,5x,f5.2,2x,f5.2,i6,2x,i6)
        call  dotcal(dtht(i),radm,nf,arcf)
        if(nf.ne.nobst(i))then
        goto 100
        endif
        goto 107
        endif
77      continue

107    continue

51     if(radm.eq.0)then
       xnumt = 0.0
       write(1,53)radm,num00
54     format(5x,f5.2,5x,f5.2,2x,f5.2,i6,2x,i6)
       goto 58
       endif 


        write(1,152)mrw,imw,nf
152     format(5x,a3,2x,i6,2x,i6)
             
53     format(5x,f5.2,5x,i6)

             L2: do  j = 1,nf
             xdot(j) = xdot(j) + xtran
             ydot(j) = ydot(j) + ytran
             zdot(j) = zdot(j) + ztran

       write(7,786)'ATOM',matw,mrw,imw,xdot(j),ydot(j),zdot(j)
786    format(a4,9x,a3,1x,a3,3x,i3,4x,3f8.3)

       write(1,157)xdot(j),ydot(j),zdot(j),xnor(j),ynor(j),znor(j)
     &,2,matw
             enddo L2
157    format(2x,3f8.3,2x,3f8.3,1x,i3,2x,a3)


58     continue

       enddo L0

       goto 102

100    continue
101    format('THERE IS SOME PROBLEM')

102    continue

       stop
       end
c---------------------------------------------
c---------------------------------------------
c---------------------------------------------
      Subroutine   angcheck(dtheta,radius,npoints,arc)

      common/const/pi
      npoints = 0 

      arc= (pi*dtheta*radius)/180.0

      do 10 i=1,50000

      if( (float((i-1))*dtheta).gt.180 )then
      goto 13
      endif


      theta =  ( (i-1)*dtheta*pi )/180.0

      if(theta.eq.0)then
      npoints = npoints + 1
      goto 10
      endif
     
      if(theta.eq.pi)then
      npoints = npoints + 1
      goto 10
      endif

      phi=arc/(radius*sin(theta))
      circum= 2.0*pi*radius*sin(theta)
      

      do  30  j=1,5000
      if( (circum.lt.arc).and.(j.eq.1) )then
      npoints = npoints + 1
      goto 10
      endif
      if(circum.lt.j*arc)then
      goto 10
      endif
      npoints = npoints + 1
30    continue

10    continue

13    continue

      return
      end
c----------------------------------------------------
c----------------------------------------------------
c----------------------------------------------------
       subroutine dotcal(dtheta,radius,nobser,arc)

       common/surface/xdot(2500),ydot(2500),zdot(2500)
       common/normal/xnor(2500),ynor(2500),znor(2500)
       common/const/pi


       do  103   i =1,2500
       xdot(i) = 0.0
       ydot(i) = 0.0
       zdot(i) = 0.0
       xnor(i) = 0.0
       ynor(i) = 0.0
       znor(i) = 0.0
      
103    continue




      arc= (pi*dtheta*radius)/180.0
      nobser =0

      do 10 i=1,50000

      if( (float((i-1))*dtheta).gt.180 )then
      goto 13
      endif


      theta =  ( (i-1)*dtheta*pi )/180.0

      if(theta.eq.0)then
      nobser = nobser +1
      xdot(nobser)=0.000
      ydot(nobser)=0.000
      zdot(nobser)=radius
      xnor(nobser)=0.000
      ynor(nobser)=0.000
      znor(nobser)=1.000
      goto 10
      endif
     
      if(theta.eq.pi)then
      nobser = nobser +1
      xdot(nobser)=0.00
      ydot(nobser)=0.00
      zdot(nobser)=-radius
      xnor(nobser)= 0.000
      ynor(nobser)= 0.000
      znor(nobser)=-1.000
      goto 10
      endif

      phi=arc/(radius*sin(theta))
      circum= 2.0*pi*radius*sin(theta)
      

      do  30  j=1,5000
      if( (circum.lt.arc).and.(j.eq.1) )then
      nobser = nobser +1
      xdot(nobser)= radius*sin(theta)
      ydot(nobser)= 0.000
      zdot(nobser)= radius*cos(theta)
      val = sqrt(xdot(nobser)*xdot(nobser)
     * + ydot(nobser)*ydot(nobser)
     * + zdot(nobser)*zdot(nobser) )
      xnor(nobser) = xdot(nobser)/val
      ynor(nobser) = ydot(nobser)/val
      znor(nobser) = zdot(nobser)/val
      goto 10
      endif

      if(circum.lt.j*arc)then
      goto 10
      endif

      nobser = nobser + 1
      xdot(nobser)= radius*sin(theta)*cos(j*phi)
      ydot(nobser)= radius*sin(theta)*sin(j*phi)
      zdot(nobser)= radius*cos(theta)
      xnor(nobser) = sin(theta)*cos(j*phi)
      ynor(nobser) = sin(theta)*sin(j*phi)
      znor(nobser) = cos(theta)
30    continue

10    continue

13    continue

       return
       end
c--------------------------------------------
c--------------------------------------------
c--------------------------------------------

