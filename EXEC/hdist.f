      program hdist
!======================================================================
!============================ HIGHEST INTERATOMIC DISTANCE IN A PROTEIN
!======================================================================
! TO FID IN GSIZE (NUMBER OF GRID POINTS IN A CUBLIC LATTICE) IN DELPHI
!======================================================================

      character(80)::ifile
      real::x(50000),y(50000),z(50000)

      call getarg(1,ifile)

      open (1,file=ifile,status='old')

      ic = 0
      do i = 1,50000
      read(1,34,end=30)x(i),y(i),z(i)
      ic = ic + 1
      enddo

30    continue

      distmax = 0.000

      do i = 1,ic
           do j = i,ic
           dist = sqrt((x(i)-x(j))**2 + (y(i)-y(j))**2 + (z(i)-z(j))**2)
                     if (dist >= distmax)then
                     distmax = dist
                     endif
            enddo
      enddo


781   format(f12.5)
34    format(30x,3f8.3)
780   format(i10)
      
! grid points / Angstrom

      scalep = 1.2               

! Number of grid points per side of the cubic lattice 

      write(444,781)distmax

      boxdim = distmax+25.0       

      write(*,781)boxdim 


      stop 
      endprogram hdist
