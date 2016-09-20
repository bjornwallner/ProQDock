!        correction for vanderwall surface complimentarity for disulphide bonds in a protein
!        cut off distance : 2.25 Angstrom 

        character(80)::pdbf
        dimension irn(20000),x(20000),y(20000),z(20000),
     1        irns(100),xs(100),ys(100),zs(100)
        character*3 rt(20000),at(20000),rcys
        character*2 asg
        character*1 ch(20000),chs(100)


        call getarg(1,pdbf)
        rcys = 'CYS'
        asg = 'SG'

        open (unit = 1,file = pdbf, status = 'old')
        open (unit = 2,file = 'dsl.out',status = 'new')
        open (unit = 3,file = 'dsl.num',status = 'new')

10      format (13x,a3,1x,a3,1x,a1,i4,4x,3f8.3)
20      format (1x,i4,2x,a3,2x,a2,2x,a1,4x,i4,2x,a3,2x,a2,2x,a1,5x,f8.3)
33      format (2x,i3)
        ic = 0
        L1 : do i = 1,20000
        read (1,10,end = 30)at(i),rt(i),ch(i),irn(i),x(i),y(i),z(i)
!        write (*,10)at(i),rt(i),ch(i),irn(i),x(i),y(i),z(i)

        ic = ic +1
        enddo L1
30        continue
        
        write(*,*)'No of atoms :', ic

        icnt = 0
        L2 : do i = 1,ic
                if (at(i) .eq. 'SG ' .and. rt(i) .eq. 'CYS')then
                icnt = icnt + 1
                irns(icnt) = irn(i)
                xs(icnt) = x(i)
                ys(icnt) = y(i)
                zs(icnt) = z(i)
                chs(icnt) = ch(i)
                endif
        enddo L2

        write(*,*)'No of CYSs :', icnt

        isuc = 0
        L3 : do i = 1,icnt
                k = i +1 
                L4 : do j = k,icnt
                        if ((chs(i).eq.chs(j).and.irns(i) /= irns(j))
     1        .or.(chs(i).ne.chs(j)))then
                        call dist (xs(i),ys(i),zs(i),xs(j),ys(j),
     1                                zs(j),dis)
                        if (dis < 2.25 .and. dis > 0)then
                       write(2,20)irns(i),rcys,asg,chs(i),irns(j),rcys,
     1        asg,chs(j),dis
                        isuc = isuc +1
                        endif
                endif
                enddo L4
        enddo L3

        write(*,*) 'No of CYS-CYSs :' , isuc
        write(3,33)isuc


        stop
        end

! ---------------------------------------------------------


        subroutine dist (x1,y1,z1,x2,y2,z2,dd)

                xsq = (x1-x2)**2
                ysq = (y1-y2)**2
                zsq = (z1-z2)**2

                dd = sqrt(xsq + ysq + zsq)

                return
        end
        
