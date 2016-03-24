#!/bin/bash

path=`echo ${0/\/EXEC\/extAllTerms.bash}`
echo $path

rosetta_repacked=$1

outfile=`echo $rosetta_repacked | cut -f1 -d'.'`.Rterms

if [ -e "$outfile" ]; then
rm $outfile;
fi

	len=`wc -l $rosetta_repacked | cut -f1 -d' '`
	I_sc=`tail -n2 $rosetta_repacked | cut -c281-290`
	rTs=`tail -n2 $rosetta_repacked | cut -c7-16`
	Fa=`tail -n2 $rosetta_repacked | cut -c18-27`
	Fr=`tail -n2 $rosetta_repacked | cut -c29-38`
	n1=`echo ${rTs/score/}`;
	n2=`echo ${Fr/fa_rep/}`;
#	echo $n1
#	echo $n2
	TmR=`$path/EXEC/TmR.pl $n1 $n2`
	fl=`echo $rosetta_repacked | cut -f2 -d'/'`
	fln=`echo ${fl/\.rosetta_repacked/\.pdb}`;
	echo "# RIsc Rts Fatr Frep Ftmr" > h1
	echo $fln     $I_sc    $rTs     $Fa     $Fr   $TmR > out
	awk '{printf "%1s %10s %12s %12s %12s %12s\n",$1,$2,$3,$4,$5,$6}' h1 >> $outfile
	awk '{printf "%12.3f %12.3f %12.3f %12.3f %12.3f\n",$3,$5,$7,$9,$10}' out >> $outfile
#	echo $n1    $n2     $TmR

	cat $outfile


