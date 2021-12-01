      program linkden
!==========================================================================
!  CALCULATE LINK DENSITY OF CONTACT NETWORKS AT Protein-Protein Interface 
!==========================================================================

      character(80)::pdbfile
      character(4)::atom(35000),atom1(35000),atom2(35000)
      character(3)::res(35000),res1(35000),res2(35000)
      real::x(35000),y(35000),z(35000)
      real::x1(35000),y1(35000),z1(35000)
      real::x2(35000),y2(35000),z2(35000)
      integer::ires(35000),ires1(35000),ires2(35000)
      character(1)::chain(35000),chain1,chain2
      character(4)::atomA(30),atomB(30)
      character(100)::string
      character(3)::resA,resB
      real::xA(30),yA(30),zA(20)
      real::xB(30),yB(30),zB(20)
      integer::itar(1000),inei(1000)
      real::ld
      real::dist2

      call getarg(1,pdbfile)

      open (1,file=pdbfile,status='old')

      write(56,125)'load ',pdbfile
      write(56,*)'spacefill'
      write(56,*)'color chain'
   
125   format(a5,a30)
      ic = 0
 
      do i = 1,35000
      read(1,24,end=20)string
          if (string(1:4).eq.'ATOM')then
          read(string,34)atom(i),res(i),chain(i),ires(i),x(i),y(i),z(i)
          write(*,34)atom(i),res(i),chain(i),ires(i),x(i),y(i),z(i)
          ic = ic + 1
          endif
      enddo

20    continue
      
      chain1 = chain(1)
      chain2 = chain(ic)
    
!      print*,chain1,'  ',chain2

24    format(a80)
34    format(12x,a4,1x,a3,1x,a1,i4,4x,3f8.3)      

      i1 = 0
      do i = 1,ic
           if (chain(i) .eq. chain1)then
           i1 = i1 + 1
           atom1(i1) = atom(i)
           res1(i1) = res(i)
           ires1(i1) = ires(i)
           x1(i1) = x(i)
           y1(i1) = y(i)
           z1(i1) = z(i)
           endif
      enddo 

      ic1 = i1

      i2 = 0
      do i = 1,ic
           if (chain(i) .eq. chain2)then
           i2 = i2 + 1
           atom2(i2) = atom(i)
           res2(i2) = res(i)
           ires2(i2) = ires(i)
           x2(i2) = x(i)
           y2(i2) = y(i)
           z2(i2) = z(i)
           endif
      enddo 

      ic2 = i2

!      print*,ic1,ic2

       itotres = (ires1(ic1)-ires1(1)) + (ires2(ic2)-ires2(1))

!==================== CALCULATE THE CONTACT NETWORK ======================
      cutoff = 6.0    ! Atomic Contact Cutoff; Exclude Hydrogens 
      Ncon = 0

      do i1 = ires1(1),ires1(ic1)
      n1 = 0
            do i = 1,ic1
                  if ((i1==ires1(i)) .and. (atom1(i)(2:2).ne.'H'))then
                  n1 = n1 + 1
                  iresA = i1
                  resA = res1(i)
                  atomA(n1) = atom1(i)
                  xA(n1) = x1(i) 
                  yA(n1) = y1(i) 
                  zA(n1) = z1(i) 
                  endif
           enddo
              do j1 = ires2(1),ires2(ic2)
              n2 = 0 
                    do j = 1,ic2
                         if ((j1==ires2(j))  .and. 
     &(atom2(j)(2:2).ne.'H'))then
                         n2 = n2 + 1
                         iresB = j1
                         resB = res2(j) 
                         atomB(n2) = atom2(j)
                         xB(n2) = x2(j)
                         yB(n2) = y2(j)
                         zB(n2) = z2(j)
                         endif
                    enddo
                    icnt = 0
                    do ii = 1,n1
                           do jj = 1,n2
                           dist = sqrt((xA(ii)-xB(jj))**2 +
     &(yA(ii)-yB(jj))**2 + (zA(ii)-zB(jj))**2)
                                if (dist <= cutoff)then 
                                icnt = icnt + 1
                                dist2=dist
                                endif
                           enddo
                    enddo
         if (icnt > 0)then
         Ncon = Ncon + 1
         itar(Ncon) = iresA
         inei(Ncon) = iresB
         write(85,98)iresA,resA,chain1,' <=> ',iresB,resB,chain2,icnt,dist2
         write(56,71)'select ',iresA,' and :',chain1
         write(56,*)'color white'
         write(56,71)'select ',iresB,' and :',chain2
         write(56,*)'color yellow'
         endif
              enddo     
      enddo     

98    format(i3,2x,a3,2x,a1,2x,a5,2x,i3,2x,a3,2x,a1,2x,i5,f8.2)
81    format(i4,2x,a3,2x,i3,5x,i4,2x,a3,2x,i3,2x,i5)
!      print*,Ncon
71    format(a7,i4,a6,a1)

!==========================================================================
!======================== TOTAL NUMBER OF LINKS : Ncon ====================
!================ FIGURE OUT MATRIX DIMENSION  ============================
!==========================================================================

      do i = 1,Ncon
      write(70,88)itar(i)
      write(71,88)inei(i)
      enddo

88    format(i5)

      call system ('sort fort.70 | uniq | wc -l > tar')      
      call system ('sort fort.71 | uniq | wc -l > nei') 

      open (2,file='tar',status='old')     
      open (3,file='nei',status='old')     
  
      read(2,135,end=40)N1
      read(3,135,end=40)N2
40    continue

135   format(i5)

      itot = N1+N2

      idimn = N1*N2
!      write(*,*)idimn

      ld = Ncon/float(idimn)
      Fintres=float(itot)/float(itotres)
      write(*,235)ld,idimn,itot,itotres,Fintres,Ncon
      write(130,236)ld

235   format(f8.5,2x,i10,2x,i5,2x,i5,2x,f8.5,i5)
236   format(f8.5)

      endprogram linkden

