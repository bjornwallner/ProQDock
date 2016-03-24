#!/bin/bash

#==============================================================================================================
# ProQDock (v.1): A Docking Scoring Program for protein complexes 
# using high-level features and Support Vector Machine (SVM) Learning 
# Current version calculates 13 Features: Sc, EC, CPM, CPscore, Ld, ISc (Interface Features),
# rGb, ProQ2, rTs, Erep, Etmr (All Atom Features).
#==============================================================================================================

#==============================================================================================================
# Currently there are three external programs used in the course of calculating the features
# Please set the following paths according to the compilation on your own machine
#==============================================================================================================

user=x_sabas
sc_path=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin
delphi_path=/home/x_sabas/bin
rosetta_path=/proj/wallner/users/x_bjowa/github/Rosetta/main/source/bin
rosetta_db=/proj/wallner/users/x_bjowa/github/Rosetta/main/database
svm_path=/home/x_sabas/proj/svm_light
ESpath=/home/x_sabas/proj/EDTSurf
proqpath=/home/x_sabas/proj/BACKUP/ProQ_scripts/bin
proqscorepath=/proj/wallner/apps/rosetta/current/main/source/bin
nseqflag=1

#==============================================================================================================

tmpdir=`mktemp -d`
#echo $tmpdir

upath=`echo ${0/\/ProQDock.bash/}`
path=`readlink -f $upath`

pdbpath=$1
pdbpath=`readlink -f $pdbpath`
pdb=`basename $pdbpath`
echo $pdb

Nchar=`echo ${#pdb}`

#echo $Nchar

startE=`echo ${Nchar}-3 | bc -l`
stopE=`echo $Nchar`

#echo $startE $stopE

extn=`echo $pdb | cut -c$startE-$stopE`

echo $extn

if [ "$extn" == ".pdb" ] || [ "$extn" == ".PDB" ]; then
echo "The file has a Proper extension (.pdb / .PDB)"
else
echo "The Input filename MUST have an extension .pdb / .PDB"
exit;
fi

echo $pdb
echo $pdbpath

atoms=`awk '$1=="ATOM"' $pdbpath | wc -l`

if [ "$atoms" == "0" ]; then 
echo "The input pdb file does not have a Brookhaven format"
exit;
fi

diel=$2
gauss=$3

echo "============================================================="
echo "EXEC PATH:" $path
echo "============================================================="

if [ "$#" -lt "1" ]; then
echo "================================================================================="
echo "Usage: $path/ProQDock.bash <PDB_filename.pdb>"
echo "Optional Argument(s): "
echo "-d (Delphi_mode) : 0 / 1 (0: Single Dielctric; 1: Multidielctric) "
echo "================================================================================="
exit;
fi

if [ "$#" -lt "2" ]; then
echo "The Multidielectric Gaussian flag is set to 'off' mode"
echo "No user-input provided for this parameter"
echo "Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed"
gauss=0
elif [ "$#" -ge "3" ]; then
        if [ "$diel" == "-d" ]; then
            if [ "$gauss" == "1" ]; then
		echo "The Multidielectric Gaussian flag is 'up'"
		echo "Delphi will commense with its Multidielctric Gaussian feature"
            elif [ "$gauss" == "0" ]; then
		echo "The Multidielectric Gaussian flag is 'down'"
		echo "Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed"
            fi
	else
	    echo "Incorrect optional argument"
	    echo "Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed"
	    gauss=0
	fi
else
echo "Incorrect optional argument"
echo "Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed"
gauss=0
fi

basename=`echo ${pdb/\.pdb/}`
basename=`echo ${basename/\.PDB/}`
outdir=OUT$basename

outf=`echo $basename`.SVMfeatures

if [ -e "$outf" ]; then
rm $outf
fi

nres=0

cloc=`pwd`
#echo "My starting Location is: $cloc"

#ls -lart $tmpdir

cp $pdbpath $tmpdir/ &> /dev/null
cd $tmpdir/

#=======================================================================
#========== delphi parameter files should be in ~/lib ==================
#===== The line restriction is 80 character in the Fortran version =====
#=======================================================================

rm -f amber.crg &> /dev/null
rm -f amber_dummy.siz &> /dev/null

ln -s $path/LIBR/amber.crg .
ln -s $path/LIBR/amber_dummy.siz .

loc=`pwd`
#echo "Currently I am at:" $loc "which is supposed to be" $tmpdir
mkdir $outdir
echo "==========================================================="

rm -f formch.out &> /dev/null
rm -f error.log &> /dev/null

$path/MAINEXEC/formcheck.pl $pdb > error.log 

if [ -e "formch.out" ]; then
  ls -lart formch.out
  cat formch.out
fi

if [ -e "formch.out" ]; then
echo "The input file does not meet all necessary conditions for the program to run"
cat $path/HELP/input.compatibility
echo "Check: error.log"
echo "============================================================="
mv error.log $cloc
mv formch.out $cloc
cd $cloc
rm -rf $tmpdir
ls -lart error.log
ls -lart formch.out
exit;
fi

rm -f redun.out &> /dev/null
rm -f rdn.log &> /dev/null

nres=`$path/MAINEXEC/pdb2resWMchain.pl $pdb`
echo "Total number of residues in the Complex: $nres"

if [ "$nres" -gt "2500" ]; then 
echo "==================================================="
echo "The input pdb file contains more than 2500 residues which exceeds the current upperlimit in size of the protein-protein complex"
echo "==================================================="
cat $path/HELP/input.compatibility
exit;
fi

if [ -e "rdn.log" ]; then 
echo "============================================================="
cat rdn.log
mv rdn.log $cloc
cd $cloc
rm -rf $tmpdir
echo "check: rdn.log"
ls -lart rdn.log
echo "============================================================="
cat $path/HELP/input.compatibility
exit;
fi

#echo "$path/MAINEXEC/runEC.bash $pdb $delphi_path $ESpath $gauss &> /dev/null"
rm -f check.Nint &> /dev/null
c1=`$path/MAINEXEC/runEC.bash $pdb $delphi_path $ESpath $gauss &> /dev/null`
if [ -e "check.Nint" ]; then
ls -lart check.Nint
  if [ `cat check.Nint` == "NO_Nint" ]; then
    echo "============================================="
    echo "There is No Interafce between the two complex 
{i.e., No atoms detected at the Interface (delta_ASA > 0) between the two chains}"
    echo "============================================="
    mv $pdb temp
    mv $basename.log temp2
    $path/intfiles.clean $basename
    mv temp2 $basename.log
    mv temp $pdb
    rm -rf $outdir
    mv $pdb $cloc
    mv $basename.log $cloc
    exit;
  fi
fi

cat $path/HELP/features.description

#======================== EC ================================
EC=`cat $basename.EC`
echo "EC="$EC
#============================================================
#========================= Sc ===============================
awk '$1=="ATOM"' $pdb > temp
mv temp $pdb
c2=`$path/MAINEXEC/runSc.bash $pdb $sc_path  &> /dev/null`
Sc=`cat $basename.Sc`
echo "Sc="$Sc
#========================= rGb ==============================
c3=`$path/MAINEXEC/calrGb.bash $pdb  &> /dev/null`
rGb=`cat $basename.rGb`
echo "rGb="$rGb
#========================== Ld =============================
c4=`$path/MAINEXEC/ldN.exe $pdb  &> /dev/null`
mv fort.130 $basename.Ld
Ld=`cat $basename.Ld`
echo "Ld="$Ld
#==================== nBSA & Fintres =======================
c5=`$path/MAINEXEC/nBSAFintres.bash $pdb &> /dev/null`
nBf=`cat $basename.nBSAFintres`
nBSA=`awk '{print $1}' <<< $nBf`
Fintres=`awk '{print $2}' <<< $nBf`
echo "nBSA="$nBSA
echo "Fintres="$Fintres
#======================= CPscore ===========================
c6=`$path/MAINEXEC/run.CPscore $pdb &> /dev/null`
CPS=`cat $basename-C0.CPS`

one=1.00
zero=0.00

# Readjust outliers based on a True(1)-False(0) Logic

gto=`echo $CPS '>' $one | bc -l`
ltz=`echo $CPS '<' $zero | bc -l`

if [ "$gto" -eq "1" ]; then 
CPS=1.00
elif [ "$ltz" -eq "1" ]; then 
CPS=0.00
fi

echo "CPscore="$CPS   

#=========================== CPM ===========================
CPM=`$path/MAINEXEC/CPMgScEC.pl $path $Sc $EC $nBSA`
echo $CPM > $basename.CPM
echo "CPM="$CPM
#echo $nres

if [ "$nres" > "0" ]; then
#==================== Rosetta terms ========================
c7=`$path/MAINEXEC/run.rossettaE $pdb $rosetta_path $rosetta_db &> /dev/null`
#cat $basename.Rterms_scaled
strR=`tail -n1 $basename.Rterms_scaled`
Isc=`awk '{print $1}' <<< $strR`
rTs=`awk '{print $2}' <<< $strR`
Eatr=`awk '{print $3}' <<< $strR`
Erep=`awk '{print $4}' <<< $strR`
Etmr=`awk '{print $5}' <<< $strR`
echo "rTs="$rTs
echo "Isc="$Isc
echo "Erep="$Erep
echo "Etmr="$Etmr
else
echo "========================================================="
echo "LENGTH passed on can not be zero"
echo "Something wrong with the " $basename.res "file"
echo "========================================================="
fi

#echo "==========================================================="
#/home/x_sabas/proj/BACKUP/ProQ_scripts/bin/run_all_external.pl -pdb $pdb -cpu 4

$path/MAINEXEC/runProQ.bash $pdb $proqpath $proqscorepath $nseqflag &> /dev/null

if [ -e "$pdb.ProQ2" ]; then
prq=`tail -n1 $pdb.ProQ2 | grep "SCORE:" | cut -c7-16`
fi

ProQ=`echo ${prq}/${nres} | bc -l`
echo $ProQ > temp
ProQ=`awk '{printf "%10.3f\n",$1}' temp`
echo "ProQ="$ProQ       

#ProQ=0.90719   # To be calculated (2djz)
#ProQ=0.80419   # To be calculated (2boj)

echo "SCORE  rGb nBSA Fintres Sc EC ProQ Isc rTs Erep Etmr CPM Ld CPscore" > h2
echo "SCORE" $rGb $nBSA $Fintres $Sc $EC $ProQ $Isc $rTs $Erep $Etmr $CPM $Ld $CPS  > out

#awk '{printf "%5s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s\n",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14}' h2
#awk '{printf "%5s  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f\n",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14}' out
#echo "==========================================================="

rm -f $out &> /dev/null

awk '{printf "%5s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s  %8s\n",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14}' h2 >> $outf
awk '{printf "%5s  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f  %8.5f\n",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14}' out >> $outf

$path/MAINEXEC/features2svm.pl $outf &> /dev/null

svmtest=$outf.svm
#ls -lart $svmtest

cnt=0
sumpred=0.00
pred5=`echo ${svmtest/\.svm/\.pred5}`
rm -f $pred5 &> /dev/null

for model in `echo $path/SVMmodels/*.model`
do
let cnt=cnt+1
pred=`echo ${svmtest/\.svm/\-$cnt.pred}`
#echo $cnt $pred
$svm_path/svm_classify  $svmtest  $model  $pred &> /dev/null
cat $pred >> $pred5
rm -f $pred
done

sumpred=`cat $pred5 | awk 'BEGIN{sum=0}{sum=sum+$1}END{print sum}'`

#echo $sumpred
ProQDock=`echo ${sumpred}/${cnt} | bc -l`

#===============================================================
#================= Readjust ProQDock outliers ==================
#======= forcefully readjust pqd > 1 to 1 and pqd < 0 to 0 =====
#===============================================================
#===============================================================

one=1.00
zero=0.00

# Readjust outliers based on a True(1)-False(0) Logic

gto=`echo $ProQDock '>' $one | bc -l`
ltz=`echo $ProQDock '<' $zero | bc -l`

rscl=0

if [ "$gto" -eq "1" ]; then 
echo "Raw ProQDock:" $ProQDock
ProQDock=1.00
rscl=1
elif [ "$ltz" -eq "1" ]; then 
echo "Raw ProQDock:" $ProQDock
ProQDock=0.00
rscl=1
fi

#===============================================================
#===============================================================
#===============================================================

outp=$basename.ProQDock
echo "================The ProQDock Score ===================="
if [ "$rscl" == "0" ]; then
echo $ProQDock > temp
awk '{printf "%10.5f\n",$1}' temp 
elif [ "$rscl" == "1" ]; then
echo $ProQDock "(readjusted)" > temp
awk '{printf "%10.5f %15s\n",$1,$2}' temp 
fi
echo "======================================================="
awk '{printf "%10.5f\n",$1}' temp > $outp

mv $basename* $outdir/

cd $outdir/
$path/intfiles.clean $basename
cp $pdbpath .
cd $tmpdir/
ls -lart $outdir

$path/clean.bash  &> /dev/null 
cp -r $outdir $cloc/

cd $cloc

ls -lart $outdir/$outp
cat $outdir/$outp

#echo "I am now back to $cloc"

echo "OUTPUT Directory" $outdir
echo "Features: " $outdir/$outf
echo "ProQDock score: " $outdir/$outp

#ls -lart
#ls -lart $tmpdir
rm -rf $tmpdir
ls -lart $outdir


