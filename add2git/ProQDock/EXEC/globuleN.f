      program globule

!     Check parameters for globular approximation of a protein

      character(80)::pdb,bury
      integer::iresexp(995)

      call getarg(1,pdb)
      call getarg(2,bury)


      open (unit=1,file=pdb,status='old')
      open (unit=2,file=bury,status='old')

!====================== READ BURIAL PROFILE =====================

      itr = 0
      iexp = 0

      do i = 1,995
      read(2,56,end=20)ires,bur
!      write(*,56)ires,bur
           if (bur >= 0.3)then
           iexp = iexp + 1
           iresexp(iexp) = ires
           endif
      itr = itr + 1
      enddo
20    continue

!      print*,itr,iexp

56    format(7x,i3,32x,f4.2)

      sumx = 0.0
      sumy = 0.0
      sumz = 0.0

      xmin = 1000.0
      xmax = 0.0
      ymin = 1000.0
      ymax = 0.0
      zmin = 1000.0
      zmax = 0.0

      ic = 0

9     read(1,34,end=30)x,y,z
      ic = ic + 1
      sumx = sumx + x
      sumy = sumy + y
      sumz = sumz + z
           if (x <= xmin)then
           xmin = x
           endif
           if (x >= xmax)then
           xmax = x
           endif
           if (y <= ymin)then
           ymin = y
           endif
           if (y >= ymax)then
           ymax = y
           endif
           if (z <= zmin)then
           zmin = z
           endif
           if (z >= zmax)then
           zmax = z
           endif
      goto 9

30    continue

      cenx = sumx/ic
      ceny = sumy/ic
      cenz = sumz/ic


!     CALCULATE FARTHEST DISTANCE OF THE OBJECT FROM THE CENTROID

      rewind(1)

      dmax = 0.0
      sumd = 0.0
      sumsqd = 0.0
      itot = 0
      ich = 0

10    read(1,134,end=40)ires,x,y,z
      itot = itot + 1
      iflag = 0
           do j = 1,iexp
                if (ires==iresexp(j))then
                iflag = 1
                endif
           enddo
           if (iflag==1)then
           goto 99
           elseif (iflag==0)then
           goto 991
           endif
99    continue
      ich = ich +1 
      d = sqrt((x-cenx)**2 + (y-ceny)**2 + (z-cenz)**2)
          sumd = sumd + d
          sumsqd = sumsqd + d**2
          if (d >= dmax)then
          dmax = d
          endif
991   continue
      goto 10

40    continue

!      print*,ich

      dmean = sumd/ich
      sigd = sqrt((sumsqd/ich)-dmean**2)

      write(3,81)sigd             

817   format(i3,2x,i3,2x,i5,2x,i5,2x,f8.3)

81    format(f8.3)

35    format(3f8.3)
36    format(2f8.3)
37    format(f8.3)
46    format(a4,2x,2f8.3)
34    format(30x,3f8.3)
134   format(23x,i3,4x,3f8.3)

      endprogram globule
