#!/bin/bash

upath=`echo ${0/\/MAINEXEC\/runSc.bash/}`
echo $upath
path=`readlink -f $upath`
echo $path
pdbinp=$1
echo $pdb
sc_path=$2
echo $sc_path
chains=`$path/EXEC/chainsplit.pl $pdbinp chain_only`
chain1=`echo $chains | cut -f1 -d'~'`
chain2=`echo $chains | cut -f2 -d'~'`

echo $chain1 $chain2

pdb_noH=pdbwH.pdb.$$
$path/EXEC/trimH.pl $pdbinp > $pdb_noH
#echo $CCP4_SCR
#echo "$path/EXEC/run_sc $pdb_noH $chain1 $chain2 $sc_path "
#$path/EXEC/run_sc $pdb_noH $chain1 $chain2 $sc_path
out=`$path/EXEC/run_sc  $pdb_noH $chain1 $chain2 $sc_path | grep "Shape complementarity statistic Sc = "`
Sc=`echo ${out/Shape complementarity statistic Sc = /}`;
echo $Sc

outfile=${pdbinp/\.pdb/\.Sc};

echo $Sc > $outfile;
#rm -f $pdb_noH
rm $CCP4_SCR/sc*


