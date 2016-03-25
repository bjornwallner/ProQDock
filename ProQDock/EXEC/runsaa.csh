#!/bin/csh -f 
#echo off

set file = $argv[1]
echo $file

set out1 = `echo $file | cut -f1 -d '.'`.saa

rm inp.saa*
rm saa.out
awk '$1=="ATOM" || $1=="HETATM"' $file > inp.pdb
./EXEC/saa1tr.exe
./EXEC/saa2tr.exe
sed -e '1,$s/*/ /g' inp.saa3 > temp		# * if any
./EXEC/saa3tr.exe
sed -e '1,$s/  NaN/0.000/g' saa.out > temp      # NaN if any

mv saa.out $out1
