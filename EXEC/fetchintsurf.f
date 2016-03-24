      program fetchintsurf

!=============================================================================================
!     Fetch interfacial dot surface points from the whole molecule
!     Conolly surafce and the collection of interfacial atoms 
!=============================================================================================

      character(80)::idsurf,iatom
      character(3)::atom1(5000),atom2(1000000)
      character(3)::res1(5000),res2(1000000)
      integer::ires1(5000),ires2(1000000)
      character(1)::chain1(5000),chain2(1000000)
      real::x(1000000),y(1000000),z(1000000)

      call getarg(1,iatom)
      call getarg(2,idsurf)

      open (1,file=iatom,status='old')
      open (2,file=idsurf,status='old')

      ic1 = 0

      read(1,31)header
31    format(a20)

      do i = 1,5000
      read(1,34,end=20)atom1(i),res1(i),ires1(i),chain1(i)
!      write(*,34)atom1(i),res1(i),ires1(i),chain1(i)
      ic1 = ic1 + 1
      enddo

20    continue

       print*,ic1

      ic2 = 0

      do j = 1,1000000
      read(2,35,end=30)atom2(j),res2(j),ires2(j),chain2(j),
     &x(j),y(j),z(j)
      write(38,29)'ATOM',atom2(j),res2(j),chain2(j),ires2(j)
     &,x(j),y(j),z(j)
      ic2 = ic2 + 1
      enddo

30    continue

34    format(a3,2x,a3,1x,i4,2x,a1)
35    format(a3,2x,a3,1x,i4,1x,a1,1x,f8.3,1x,f8.3,1x,f8.3)

      print*,ic1,ic2
      itot = 0

      do i = 1,ic1
           do j = 1,ic2
                if ((atom1(i).eq.atom2(j)).and.(res1(i).eq.res2(j))
     &.and.(ires1(i)==ires2(j)))then
                write(28,29)'ATOM',atom2(j),res2(j),chain2(j),ires2(j)
     &,x(j),y(j),z(j)
                itot = itot + 1
                endif
            enddo
       enddo

29      format(a4,9x,a3,1x,a3,1x,a1,i4,4x,3f8.3)



      endprogram fetchintsurf
