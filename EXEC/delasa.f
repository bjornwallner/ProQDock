      program delsaa

      character(80)::ifile1,ifile2,ifile3

      integer::ires1(20000),ires2(20000),ires3(20000)
      character(1)::chain1(20000),chain2(20000),chain3(20000)
      character(3)::res1(20000),res2(20000),res3(20000)
      character(3)::atom1(20000),atom2(20000),atom3(20000)
      real::saa1(20000),saa2(20000),saa3(20000)

      integer::ires_int1(20000),ires_int2(20000)
      character(3)::atom_int1(20000),atom_int2(20000)
      character(3)::res_int1(20000),res_int2(20000)
      character(1)::chain_int1(20000),chain_int2(20000)
      real::dls1(20000),dls2(20000)

      call getarg(1,ifile1)
      call getarg(2,ifile2)
      call getarg(3,ifile3)

      open(1,file=ifile1,status='old')
      open(2,file=ifile2,status='old')
      open(3,file=ifile3,status='old')

!     print*,ifile1
!     print*,ifile2
!     print*,ifile3

      ic1 = 0
      do i = 1,20000
      read(1,34,end=10)atom1(i),res1(i),chain1(i),ires1(i),saa1(i)
      ic1 = ic1 + 1
      enddo

10    continue

      ic2 = 0
      do i = 1,20000
      read(2,34,end=20)atom2(i),res2(i),chain2(i),ires2(i),saa2(i)
      ic2 = ic2 + 1
      enddo

20    continue

      ic3 = 0
      do i = 1,20000
      read(3,34,end=30)atom3(i),res3(i),chain3(i),ires3(i),saa3(i)
      ic3 = ic3 + 1
      enddo

30    continue

34    format(13x,a3,1x,a3,1x,a1,1x,i3,28x,f8.3)

!      print*,ic1,ic2,ic3

      int1 = 0

      do i = 1,ic1
           do j = 1,ic3
                if ((atom1(i).eq.atom3(j)).and.(ires1(i)==ires3(j)).and.
     &(res1(i).eq.res3(j)).and.(chain1(i).eq.chain3(j)))then
                   dsaa = abs(saa1(i)-saa3(j))
!                   print*,dsaa
                      if (dsaa > 0.00)then
                      int1 = int1 + 1
                      atom_int1(int1) = atom1(i)
                      res_int1(int1) = res1(i)
                      ires_int1(int1) = ires1(i)
                      chain_int1(int1) = chain1(i)
                      dls1(int1) = dsaa
 !                    print*,dls1(int1)
                      endif 
                endif
           enddo
      enddo

      int2 = 0

      do i = 1,ic2
           do j = 1,ic3
                if ((atom2(i).eq.atom3(j)).and.(ires2(i)==ires3(j)).and.
     &(res2(i).eq.res3(j)).and.(chain2(i).eq.chain3(j)))then
                   dsaa = abs(saa2(i)-saa3(j))
 !                  print*,dsaa
                      if (dsaa > 0.00)then
                      int2 = int2 + 1
                      atom_int2(int2) = atom2(i)
                      res_int2(int2) = res2(i)
                      ires_int2(int2) = ires2(i)
                      chain_int2(int2) = chain2(i)
                      dls2(int2) = dsaa
 !                     print*,dls2(int2)
                      endif 
                endif
           enddo
      enddo

      int_tot = int1 + int2


!      print*,int1,int2

      write(14,67)

      sumsaa1 = 0.000 
      sumsaa2 = 0.000 

      do i = 1,int1
      write(14,56)atom_int1(i),res_int1(i),ires_int1(i),chain_int1(i),
     &dls1(i)
      sumsaa1 = sumsaa1 + dls1(i)
      enddo

      write(15,68)
               
      do i = 1,int2
      write(15,56)atom_int2(i),res_int2(i),ires_int2(i),chain_int2(i),
     &dls2(i)
      sumsaa2 = sumsaa2 + dls2(i)
      enddo

      saamean = (sumsaa1 + sumsaa2)/2.0

      write(16,134) 'Mean ASA buried upon complexation per chain',
     &' (Ang^2):',saamean

134   format(a43,a9,2x,f12.5)

      write(*,812)int_tot,sumsaa1,sumsaa2,saamean
812   format(i6,2x,f10.3,2x,f10.3,2x,f10.3)

56    format(a3,2x,a3,2x,i3,2x,a1,2x,f9.4)
67    format('INTERFACE 1 OF THE COMPLEX')
68    format('INTERFACE 2 OF THE COMPLEX')

      endprogram delsaa


