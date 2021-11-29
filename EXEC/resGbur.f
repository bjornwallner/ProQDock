      program calresGbur

!==================================================================
!  Calculate  avg(log(prop(Res(i)|Bur(i)))) given a burial profile
!==================================================================

      character(19)::res(19),rt
      real::pr(19,4)
      character(80)::bfile
      character(256)::library_file
      call getarg(1,bfile)
      call getarg(2,library_file)


!===========================
!    READ resGbur LIBRARY
!===========================
      open (unit=1,file=library_file,status='old')

      do i = 1,2
      read(1,45,end=30)
      enddo

30    continue

45    format(/)

      do i = 1,19
       read(1,46,end=40)res(i),pr(i,1),pr(i,2),pr(i,3),pr(i,4)
!       write(*,46)res(i),pr(i,1),pr(i,2),pr(i,3),pr(i,4) 
      enddo
40    continue

46    format(a3,36x,4f8.3) 

!======================================================================
!     READ BURIAL PROFILE AND CALCULATE THE SCORE
!     Prop(i) = P(Res(i)|bur(i))/P(Res(i)|DB(i))
!     Pscore = sum_i=1,N(log(Prop(i)))/N      # log to the base 10
!======================================================================

      open(unit=2,file=bfile,status='old')

      sump = 0.00
      Nres = 0
      itot = 0
      ib1 = 0
      ib2 = 0
      ib3 = 0
      ib4 = 0

      do i = 1,3000
      read(2,61,end=50)rt,bur
      prop = 0.000
      itot = itot + 1
!======================= distance measure based on burial distribution =======
        if (bur <= 0.05)then
        ib1 = ib1 + 1
        elseif (bur > 0.05 .and. bur <= 0.15)then
        ib2 = ib2 + 1
        elseif (bur > 0.15 .and. bur <= 0.30)then
        ib3 = ib3 + 1
        elseif (bur > 0.30)then
        ib4 = ib4 + 1
        endif
!=============================================================================
          if (rt .ne. 'GLY')then
          ibtag = 4
          Nres = Nres + 1 
              if (bur <= 0.05)then
              ibtag = 1
              elseif (bur > 0.05 .and. bur <= 0.15)then
              ibtag = 2
              elseif (bur > 0.15 .and. bur <= 0.30)then
              ibtag = 3 
              elseif (bur > 0.30)then
              ibtag = 4
              endif
          do j = 1,19
               if (rt .eq. res(j))then
               prop = pr(j,ibtag)
               endif
          enddo
          write(*,715)rt,bur,ibtag,prop
          sump =  sump + ((log(prop)/log(10.0)))
          endif
      enddo

50    continue
!61    format(4x,a3,5x,f4.2)
61    format(15x,a3,24x,f4.2)
715   format(a3,2x,f4.2,2x,i3,2x,f8.3)

      pscore = sump/float(Nres)

      write(*,689)pscore

89    format(3f10.5)
689   format(f10.5)

      endprogram calresGbur
