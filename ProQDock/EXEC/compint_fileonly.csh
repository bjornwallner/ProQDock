#!/bin/csh -f

echo off

        if ($argv[1] == '-inp')then
        set pdbinp = $argv[2]
        endif

        echo 'Filename you entered:' $pdbinp
        rm -f chfn.inp
        echo $pdbinp > chfn.inp
        set fnch = `./EXEC/chfn.pl`
        goto run

#============================================================================================================================

run:


set code = `echo $pdbinp | cut -f1 -d"."`
echo $code

runcont:

#=========================================
# REFRESH DIRECTORY (remove old files if present) 
#=========================================
rm -f alter.inp
rm -f surf_dot
rm -f surf_out
rm -f surf.pdb
rm -f check
rm -f sg.out
rm -f sg.con
rm -f icon.out
rm -f num.out
rm -f remove.out
rm -f inp.pdb
rm -f sphere.dot
rm -f bury.scrt
rm -f bury.out
rm -f sucal1.out
rm -f out.satv
rm -f dsl.out
rm -f dsl.num
rm -f *.log
rm -f *.pot
rm -f *cout.pdb
rm -f *sout.pdb
rm -f inp*
rm -f dsl.num
rm -f dsl.out
rm -f ARCDAT
rm -f core
rm -f ARCDAT
rm -f fort*
rm -f outhiscysO.pdb
rm -f outhisO.pdb
rm -f hiscysNC.pdb
rm -f msph.dot
rm -f numM.res
rm -f met.cores
rm -f fpp.status
rm -f formp.pdb
rm -f formch.out
rm -f redun.out
rm -f res1.out
rm -f temp*
rm -f target.res
rm -f res.replace
rm -f script1.prm
rm -f script2.prm
rm -f chfn.inp
rm -f sres.res
rm -f smres.res
rm -f smresE.res
#=========================================

rm -f redun.out
rm -f rdn.log


cp $pdbinp orig.pdb
set NO = `./EXEC/pdb2resWMchain.pl orig.pdb`
wc -l orig.res

set chains = `./EXEC/chainsplit.pl $pdbinp`
set chain1 = `echo $chains | cut -f1 -d'~'`
set chain2 = `echo $chains | cut -f2 -d'~'`

echo "TWO POLYPEPTIDE CHAINS FOUND IN THE INPUT MOLECULE: CHAIN" \'$chain1\' "AND CHAIN" \'$chain2\'

set pdb1 = `echo $code`_`echo $chain1`.pdb
set pdb2 = `echo $code`_`echo $chain2`.pdb
set pdb12 = `echo $code`_`echo $chain1``echo $chain2`.pdb

echo "PDB FILE CONTAINING THE FIRST CHAIN :" $pdb1
echo "PDB FILE CONTAINING THE SECOND CHAIN:" $pdb2
echo "PDB FILE CONTAINING BOTH THE CHAINS :" $pdb12

cp inp1.pdb $pdb1
cp inp2.pdb $pdb2
cp $pdbinp $pdb12

#===================================================================
# CRAETE OUTPUT DIRECTORY
#===================================================================

set outdir = OUT`echo $code`
rm -rf $outdir/
mkdir $outdir/

#=================================================================
#  MAP PDB FROM RES 1 (ALREADY MAPPED)
#=================================================================
echo "==============================================================="

cp $pdb1 inp1map.pdb
set N1 = `./EXEC/pdb2resWMchain.pl inp1map.pdb`
echo "$pdb1 CONTAINS $N1 RESIDUES"
cp $pdb2 inp2map.pdb
set N2 = `./EXEC/pdb2resWMchain.pl inp2map.pdb`
echo "$pdb2 CONTAINS $N2 RESIDUES"
cp $pdb12 inp12map.pdb
set N3 = `./EXEC/pdb2resWMchain.pl inp12map.pdb`
echo "$pdb12 CONTAINS $N3 RESIDUES"

echo "==============================================================="

set lench = `wc -l inp12map.res | cut -f1 -d' '`

echo "==============================================================="
echo "CHECKING CAHIN-LENGTH"
echo "==============================================================="

goto PROCEED1

PROCEED1:

#=======================================================================
# SET FIELDS (FILENAMES)
#=======================================================================

echo "==============================================================="
echo "SETTING FIELDS"
echo "==============================================================="

set asa1 = `echo $pdb1 | cut -f1 -d '.'`.asa
set asa2 = `echo $pdb2 | cut -f1 -d '.'`.asa
set asa12 = `echo $pdb12 | cut -f1 -d '.'`.asa
set asa1log = `echo $pdb1 | cut -f1 -d '.'`.asalog
set asa2log = `echo $pdb2 | cut -f1 -d '.'`.asalog
set asa12log = `echo $pdb12 | cut -f1 -d '.'`.asalog
set glbl = `echo $code`.glbl
set dslf = `echo $code`.dsl
set Smfile = `echo $code`.Sm
set Emfile = `echo $code`.Em
set CSplot = `echo $code`.CSplot

set bur12 = `echo $code`.bury
set brst12 = `echo $code`.brst

set surf1 = `echo $pdb1 | cut -f1 -d '.'`-surf.pdb
set surf2 = `echo $pdb2 | cut -f1 -d '.'`-surf.pdb

set intf1 = `echo $pdb1 | cut -f1 -d '.'`-intf.res
set intf2 = `echo $pdb2 | cut -f1 -d '.'`-intf.res

set surf1forsatv = `echo $pdb1 | cut -f1 -d '.'`.surf
set surf2forsatv = `echo $pdb2 | cut -f1 -d '.'`.surf
set surf12forsatv = `echo $pdb12 | cut -f1 -d '.'`.surf

set int1surf = `echo $pdb1 | cut -f1 -d '.'`.isurf
set int2surf = `echo $pdb2 | cut -f1 -d '.'`.isurf

set rasscript = `echo $code`-intsurf.spt
set metcores = `echo $code`.mco
set metrasview = `echo $code`-met.spt
set surf = `echo $code`-surf.pdb

#=========================================================================
#  CALCULATE SOLVENT ACCESSIBLE SURFACE AREA AND DETERMINE THE INTERFACE
#  INTERFACIAL ATOMS SHOULD HAVE DELSAA = (SAA(FREE) - SAA(BOUND)) != 0 
#  GENERATE INTERFACE RES FILES
#=========================================================================

echo "==============================================================="
echo "CALCULATING SOLVENT ACCESSIBLE AREAS AND DETTERMINING INTERFACE"
echo "==============================================================="

set rescut = `./EXEC/rescut.pl $pdbinp`
#echo $rescut

./EXEC/naccess.bash $pdb1	# A.pdb (original)
./EXEC/naccess.bash $pdb2	# B.pdb (original)
./EXEC/naccess.bash $pdb12	# AB.pdb (original)

set Nintstr = `./EXEC/delasa.exe $asa1 $asa2 $asa12` 
echo $Nintstr > temp

set Nint = `awk '{print $1}' temp`

echo $Nint

echo "NUMBER OF INTERFACIAL ATOMS BURIED UPON COMPLEXATION:" $Nint

if ($Nint == 0)then
echo "================================================================================================================="
echo "================================================================================================================="
echo "================================================================================================================="
echo "NO INTERFACIAL ATOMS DETECTED. THE CHAINS ARE NON-INTERACTING AS IT SEEMS. HAVE A LOOK AT THE PDB FILE in RASMOL"
echo "$code.pdb: NO INTERFACIAL ATOMS DETECTED. THE CHAINS ARE NON-INTERACTING AS IT SEEMS. HAVE A LOOK AT THE PDB FILE IN RASMOL" 
echo "================================================================================================================="
echo "================================================================================================================="
echo "================================================================================================================="
exit
endif

#outfile: fort.14, fort.15, fort.16

echo "=============================================================="
echo "=============================================================="
cat fort.16
cp fort.16 $code.asaAngsq
echo "=============================================================="
echo "=============================================================="

rm ras*.select

#wc -l inp12map.res
#wc -l orig.res

./EXEC/crmap.pl
./EXEC/intres.pl
#outfile: intf1.res, intf2.res, ras.select

mv intf1.res $intf1
mv intf2.res $intf2

./EXEC/contpref20CB.exe $pdb12 $intf1 $intf2

mv fort.35 $code.iconmat20
mv fort.36 $code.fconmat20
mv fort.135 $code.iconmat6
mv fort.136 $code.fconmat6
mv fort.37 $code.contpref20

mv $code.fconmat20 temp.fconmat20
mv $code.fconmat6 temp.fconmat6
mv $code.contpref20 temp.contpref20
#mv $pdbinp $outdir/
#mv $code*.res $outdir/
#rm $code*

#rm -rf $outdir/

mv temp.fconmat20 $code.fconmat20
mv temp.fconmat6 $code.fconmat6
mv temp.contpref20 $code.contpref20

#rm $code.fconmat20
#rm $code.fconmat6

#cat fort.36


rm -f orig.pdb
rm -f orig.res
rm -f res1.out
rm -f temp2
rm -f temp1
rm -f inp1.pdb
rm -f inp2.pdb
rm -f inp1map.pdb
rm -f inp1map.res
rm -f inp2map.pdb
rm -f inp2map.res
rm -f inp12map.pdb
rm -f redun.out
rm -f rdn.log
rm -f inp12map.res
rm -f met.cores
rm -f met.rasview
rm -f fort.14
rm -f fort.15
rm -f fort.16
rm -f reso2n.map
rm -f ras1.select
rm -f ras2.select
rm -f fort.23
rm -f fort.31










