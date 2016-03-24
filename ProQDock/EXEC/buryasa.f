c      program to analyse buried residues   (ONLY WORKS FOR A SINGLE POLYPEPTIDE CHAIN)
!=======================================================================================
!  MODIFIED TO WORK ON NACCESS OUTPUT (IN PDB FORMAT WHERE THE OCCUPANCY AND B-FACTOR COLUMNS
!  STAND FOR ASA AND VDW RADII RESPECTIVELY
!=======================================================================================


       character(80)::asafile
       character*3 res
       character*3 re
       character*1 ch
       character*1 chain
       common/chb1/sumac(2500),ires(2500),res(2500),ratio(2500),
     &chain(2500)

       call getarg(1,asafile)

       open(unit=1,file=asafile,status='old')
       open(unit=2,file='bury.out',status='new')
c*************************************************************
c*************************************************************
c*************************************************************
       cutoff=7.05
c*************************************************************
c*************************************************************
c**************************************************************
       do 50 i=1,2500
       ires(i)=0
       sumac(i)=0.0
       res(i)='   '
       chain(i)=' '
50     continue

       irt=0
       irb=0
20     read(1,10,end=100)re,ch,ir,sum
       write(9,10)re,ch,ir,sum
10     format(17x,a3,1x,a1,i4,28x,f8.3)
       sumac(ir)=sumac(ir)+sum
       res(ir)=re
       chain(ir)=ch
       ires(ir)=ir
       write(9,10)res(ir),chain(ir),ires(ir),sumac(ir)
       goto 20

100    continue

       call data(1)

       do 30 i=1,2500
!====================================================
!========Reset >1 to 1 ==============================
!====================================================
          if (ratio(i) > 1.00)then
          ratio(i) = 1.00
          endif         
       if((ires(i).eq.0).and.(res(i).eq.'   '))goto 30
       irt=irt+1
       if(ratio(i).gt.cutoff)goto 30
       write(2,60)ires(i),res(i),chain(i),sumac(i),ratio(i)
60     format(5x,i5,5x,a3,1x,a1,3x,f9.2,5x,f9.2)
       irb=irb+1
30     continue
       open(unit=4,file='bury.scrt',status='new')
       write(4,35)irt,irb
35     format(1x,i6,i6)
       stop
       end
c==========================================================

       subroutine  data(n)
       character*3 res
       character*1 chain
       common/chb1/sumac(2500),ires(2500),res(2500),ratio(2500),
     &chain(2500)

       do 90   i=1,2500
       if(ires(i).eq.0)goto 90

       if(res(i).eq.'ALA')then
       ratio(i)=sumac(i)/123.4115
       endif

       if(res(i).eq.'ARG')then
       ratio(i)=sumac(i)/256.8007
       endif

       if(res(i).eq.'ASN')then
       ratio(i)=sumac(i)/161.8336
       endif

       if(res(i).eq.'ASP')then
       ratio(i)=sumac(i)/163.7097
       endif


       if(res(i).eq.'CYS')then
       ratio(i)=sumac(i)/147.4431
       endif


       if(res(i).eq.'GLU')then
       ratio(i)=sumac(i)/195.2893
       endif


       if(res(i).eq.'GLN')then
       ratio(i)=sumac(i)/195.3820
       endif


       if(res(i).eq.'GLY')then
       ratio(i)=sumac(i)/93.5631
       endif


       if(res(i).eq.'HIS')then
       ratio(i)=sumac(i)/198.1719
       endif

       if(res(i).eq.'ILE')then
       ratio(i)=sumac(i)/179.6625
       endif


       if(res(i).eq.'LEU')then
       ratio(i)=sumac(i)/193.7956
       endif


       if(res(i).eq.'LYS')then
       ratio(i)=sumac(i)/223.2443
       endif


       if(res(i).eq.'MET')then
       ratio(i)=sumac(i)/217.3316
       endif


       if(res(i).eq.'PHE')then
       ratio(i)=sumac(i)/203.8527
       endif


       if(res(i).eq.'PRO')then
       ratio(i)=sumac(i)/159.6743
       endif


       if(res(i).eq.'SER')then
       ratio(i)=sumac(i)/135.3496
       endif

       if(res(i).eq.'THR')then
       ratio(i)=sumac(i)/155.6512
       endif


       if(res(i).eq.'TYR')then
       ratio(i)=sumac(i)/234.5125
       endif

       if(res(i).eq.'TRP')then
       ratio(i)=sumac(i)/252.3393
       endif

       if(res(i).eq.'VAL')then
       ratio(i)=sumac(i)/163.5877
       endif


90     continue
       return
       end
c------------------------------------------------------------
