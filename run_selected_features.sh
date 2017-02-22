#!/bin/bash
input_pdb=`readlink -f $1`;
tmpdir=`mktemp -d`
pdb="$$.pdb"
basename=`echo ${pdb/\.pdb/}`
fullpath=`readlink -f $0`
path=`dirname $fullpath`

#echo $fullpath
#echo $basename
#echo $path

cd $tmpdir
#cp $input_pdb $pdb
egrep '^ATOM|^TER' $input_pdb > $pdb
#echo $tmpdir


sc_path=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin
delphi_path=/proj/wallner/users/x_bjowa/local/DelPhi_Linux_SP_F95/
#delphi_path=/proj/wallner/users/x_bjowa/local/bin/
rosetta_path=/proj/wallner/users/x_bjowa/github/Rosetta/main/source/bin
rosetta_db=/proj/wallner/users/x_bjowa/github/Rosetta/main/database
svm_path=/proj/wallner/apps/svm_light
#svm_path=/proj/wallner/users/x_bjowa/local/svm_light
ESpath=/proj/wallner/users/x_bjowa/local/EDTSurf
proqpath=/proj/wallner/users/x_bjowa/github/ProQ_scripts/bin/
proqscorepath=$rosetta_path

module add ccp4/6.4.0-module &> /dev/null

#========================= Sc ===============================
#awk '$1=="ATOM"' $pdb > $basename.pdb.temp
#mv $basename.pdb.temp $pdb
#echo "$path/MAINEXEC/runSc.bash $pdb $sc_path";
c2=`$path/MAINEXEC/runSc.bash $pdb $sc_path  &> /dev/null`
Sc=`cat $basename.Sc | sed s/'\s*'/''/g`
echo "Sc =" $Sc
#pwd
#exit
#ls $pdb
#==================== nBSA & Fintres =======================
#echo "$path/MAINEXEC/nBSAFintres.bash $pdb";
c5=`$path/MAINEXEC/nBSAFintres.bash $pdb &> /dev/null`
nBf=`cat $basename.nBSAFintres`
nBSA=`awk '{print $1}' <<< $nBf | sed s/'\s*'/''/g`
Fintres=`awk '{print $2}' <<< $nBf | sed s/'\s*'/''/g`
echo "nBSA =" $nBSA
echo "Fintres =" $Fintres
#ls $pdb
#exit
#========================== Ld =============================
#echo $path/MAINEXEC/ldN.exe $pdb
c4=`$path/MAINEXEC/ldN.exe $pdb &> /dev/null`
mv fort.130 $basename.Ld
Ld=`cat $basename.Ld | sed s/'\s*'/''/g`
echo "Ld =" $Ld
#exit
#========================= rGb ==============================
#echo "$path/MAINEXEC/calrGb.bash $pdb"
c3=`$path/MAINEXEC/calrGb.bash $pdb  &> /dev/null`
rGb=`cat $basename.rGb | sed s/'\s*'/''/g`
echo "rGb =" $rGb
#exit
#ls $pdb
#=============== Contact preference (input to CPscore) ======== 
#echo "$path/EXEC/compint_fileonly.bash.fast $pdb"
tmp=`$path/EXEC/compint_fileonly.bash.fast.all $pdb`
CP20=`echo $pdb | cut -f1 -d'.'`.contpref20
C20=`echo $pdb | cut -f1 -d'.'`.iconmat20
F20=`echo $pdb | cut -f1 -d'.'`.fconmat20
contpref=`$path/EXEC/matrix2string.pl $CP20`
comp=`$path/EXEC/matrix_frac.pl $C20`
#cont=`$path/EXEC/matrix2string.pl $C20`
frac=`$path/EXEC/matrix2string.pl $F20`
echo "INTCOMP = " $comp
echo "INTPAIR = " $frac
#echo "CONTFRAC = " $frac
#echo "CONTPREF = " $contpref
#echo $tmpdir$CP20
#echo "$path/MAINEXEC/run.CPscore $pdb $svm_path"
#c6=`$path/MAINEXEC/run.CPscore $pdb $svm_path &> /dev/null`
#exit
#rm -fr $tmpdir
#exit
#ls $pdb
#pwd
#exit

rm -fr $tmpdir
exit
#ILE      1
#VAL      2
#LEU      3
#PHE      4
#CYS      5
#MET      6
#ALA      7
#GLY      8
#THR      9
#SER     10
#TRP     11
#TYR     12
#PRO     13
#HIS     14
#GLU     15
#GLN     16
#ASP     17
#ASN     18
#LYS     19
#ARG     20
