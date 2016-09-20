#!/bin/bash

upath=`echo ${0/\/MAINEXEC\/runSc.bash/}`
echo $upath
path=`readlink -f $upath`
echo $path
pdbinp=$1
echo $pdb
sc_path=$2
echo $sc_path
chains=`$path/EXEC/chainsplit.pl $pdbinp`
chain1=`echo $chains | cut -f1 -d'~'`
chain2=`echo $chains | cut -f2 -d'~'`

echo $chain1 $chain2

$path/EXEC/trimH.pl $pdbinp > pdbwH.pdb

out=`$path/EXEC/run_sc pdbwH.pdb $chain1 $chain2 $sc_path | grep "Shape complementarity statistic Sc = "`
Sc=`echo ${out/Shape complementarity statistic Sc = /}`;
echo $Sc

outfile=${pdbinp/\.pdb/\.Sc};

echo $Sc > $outfile;
rm -f pdbWH.pdb

rm $CCP4_SCR/sc*


