      program intf2surf 

      character(80)::intf,surf
      character(3)::res(2500),resf(2500),res1
      character(1)::chainf(2500),chn1,chn(2500)
      integer::ires(2500),iresf(2500),ino(2500)
      real::x(2500,5000),y(2500,5000),z(2500,5000)
      real::dl(2500,5000),dm(2500,5000),dn(2500,5000)
      integer::itype(2500,5000)
      character(3)::ato(2500,5000)

      call getarg(1,intf)
      call getarg(2,surf)

      open (1,file=intf,status='old')
      open (2,file=surf,status='old')


      ic = 0
      do i = 1,2500
      read(1,34,end=10)iresf(i),resf(i),chainf(i)
      ic = ic + 1
      enddo

10    continue

34    format(i4,1x,a3,1x,a1)

!       print*,ic

        itot=0

        do  25 i=1,2500
        itot=itot+1
        read(2,40,end=20)res1,ires1,ino1,chn1
!       write(21,40)res1,ires1,ino1,chn1
40      format(5x,a3,2x,i6,2x,i6,2x,a1)

        res(itot)=res1
        ires(itot)=ires1
        ino(itot)=ino1
        chn(itot)=chn1

        do  37  j=1,ino(itot)
        read(2,45)x(itot,j),y(itot,j),z(itot,j),
     *dl(itot,j),dm(itot,j),dn(itot,j),itype(itot,j),ato(itot,j)
!       write(21,45)x(itot,j),y(itot,j),z(itot,j),
!    *dl(itot,j),dm(itot,j),dn(itot,j),itype(itot,j),ato(itot,j)
45      format(2x,3f8.3,2x,3f8.3,1x,i3,2x,a3)
37      continue

25      continue
20      continue
c----------------------------------------------------------------


!       print*,itot

        ihit = 0

        do i = 1,ic
             do j = 1,itot
                  if ((iresf(i)==ires(j)).and.(resf(i).eq.res(j))
     &.and.(chainf(i).eq.chn(j)))then
                  ihit = ihit + 1
                  write(22,40)res(j),ires(j),ino(j),chn(j)
                  do  39  k=1,ino(j)
                  write(22,45)x(j,k),y(j,k),z(j,k),
     *dl(j,k),dm(j,k),dn(j,k),itype(j,k),ato(j,k)
39                continue

                  endif
              enddo
        enddo

!        print*,ihit


      endprogram intf2surf 
