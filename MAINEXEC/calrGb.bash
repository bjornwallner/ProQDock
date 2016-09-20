#!/bin/bash -x 

upath=`echo ${0/\/MAINEXEC\/calrGb.bash/}`
echo $upath

path=`readlink -f $upath`
echo $path

#exit;

pdbinp=$1            # original AB.pdb (full of coordinates with two chains)
echo $pdbinp

tar=`echo $pdbinp | cut -f1 -d'-'`
echo $tar

chains=`$path/EXEC/chainsplit.pl $pdbinp`   # creates inp1.pdb, inp2.pdb
chain1=`echo $chains | cut -f1 -d'~'`
chain2=`echo $chains | cut -f2 -d'~'`

echo $chain1
echo $chain2

code=`echo $pdbinp | cut -f1 -d'.'`
echo $code

if [ -e "inp1.pdb" ] && [ -e "inp2.pdb" ]; then
echo "Both inp1.pdb and inp2.pdb found to be present"
ls inp1.pdb
ls inp2.pdb
$path/EXEC/mappdb.pl $path inp1.pdb inp2.pdb
else
echo "At least one of the two files: inp1.pdb and inp2.pdb is not found"
exit;
fi

#=======================================================================================================
#=======================================================================================================
#=======================================================================================================
#=======================================================================================================
#MAP FILE: reso2n.map
#MAPPED PDB1  : inp1map.pdb & CORRESPONDING RESFILE: inp1map.res
#MAPPED PDB2  : inp2map.pdb & CORRESPONDING RESFILE: inp2map.res
#MAPPED PDB12 : inp12map.pdb  & CORRESPONDING RESFILE: inp12map.res
#=======================================================================================================

pdb1=`echo $code`_`echo $chain1`.pdb
pdb2=`echo $code`_`echo $chain2`.pdb
pdb12=`echo $code`_`echo $chain1``echo $chain2`.pdb

cp inp1map.pdb $pdb1
cp inp2map.pdb $pdb2
cp inp12map.pdb $pdb12

echo $pdb1
echo $pdb2
echo $pdb12

#=================================================================
#  MAP PDB FROM RES 1 (ALREADY MAPPED)
#=================================================================
echo "==============================================================="
echo "==============================================================="

lench=`wc -l inp12map.res | cut -f1 -d' '`

echo "==============================================================="
echo "CHECKING CAHIN-LENGTH"
echo "==============================================================="

echo "==============================================================="
echo "SETTING FIELDS"
echo "==============================================================="

asa1=`echo $pdb1 | cut -f1 -d '.'`.asa
asa2=`echo $pdb2 | cut -f1 -d '.'`.asa
asa12=`echo $pdb12 | cut -f1 -d '.'`.asa
asa1log=`echo $pdb1 | cut -f1 -d '.'`.asalog
asa2log=`echo $pdb2 | cut -f1 -d '.'`.asalog
asa12log=`echo $pdb12 | cut -f1 -d '.'`.asalog
rGbout=`echo $code`.rGb
echo $rGbout
bur12=`echo $pdb12 | cut -f1 -d '.'`.bury
brst12=`echo $pdb12 | cut -f1 -d '.'`.brst

#=========================================================================
#  CALCULATE SOLVENT ACCESSIBLE SURFACE AREA AND DETERMINE THE INTERFACE
#  INTERFACIAL ATOMS SHOULD HAVE DELSAA=(SAA(FREE) - SAA(BOUND)) != 0 
#  GENERATE INTERFACE RES FILES
#=========================================================================

echo "==============================================================="
echo "CALCULATING SOLVENT ACCESSIBLE AREAS AND DETTERMINING INTERFACE"
echo "==============================================================="

$path/EXEC/naccess.bash $pdb1	# A.pdb (original)
$path/EXEC/naccess.bash $pdb2	# B.pdb (original)
$path/EXEC/naccess.bash $pdb12	# AB.pdb (original)

$path/EXEC/delasa.exe $asa1 $asa2 $asa12 > temp.$$
Nint=`awk '{print $1}' temp.$$`
echo $Nint
#exit

zero=0;

if [ $Nint -eq $zero ]; then
echo "================================================================================================================="
echo "================================================================================================================="
echo "================================================================================================================="
echo "NO INTERFACIAL ATOMS DETECTED. THE CHAINS ARE NON-INTERACTING AS IT SEEMS. HAVE A LOOK AT THE PDB FILE in RASMOL"
echo "$code.pdb: NO INTERFACIAL ATOMS DETECTED. THE CHAINS ARE NON-INTERACTING AS IT SEEMS. HAVE A LOOK AT THE PDB FILE IN RASMOL" > $code.log
#mv $code.log ./STORErgb-CAPRI/
echo "================================================================================================================="
echo "================================================================================================================="
echo "================================================================================================================="
exit
fi

#outfile: fort.14, fort.15, fort.16

echo "=============================================================="
echo "=============================================================="
cat fort.16
echo "=============================================================="
echo "=============================================================="

#=========================================================================
# CALCULATE BURIAL OF RESIDUES AS IN THE WHOLE COMPLEX
#=========================================================================

echo "==============================================================="
 echo "CALCULATING BURIAL OF RESIDUES AS IN THE WHOLE (COMPLEX) MOLECULE"
echo "==============================================================="

rm bury.out
rm bury.scrt
$path/EXEC/buryasa.exe $asa12
echo $path/EXEC/buryasa.exe $asa12
ls bury.out
cp bury.out $bur12

hydbur=`$path/EXEC/rGb.exe $bur12`
echo $hydbur 
echo $hydbur > $rGbout

rm inp*
rm fort.*
rm int*
rm temp*

