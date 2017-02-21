#!/bin/bash

upath=`echo ${0/\/MAINEXEC\/nBSAFintres.bash/}`
#echo $upath
path=`readlink -f $upath`
#echo $path


pdbinp=$1
#echo $pdbinp

outfile=`echo ${pdbinp/\.pdb/\.nBSAFintres}` 
#echo $outfile

chains=`$path/EXEC/chainsplit.pl $pdbinp`
chain1=`echo $chains | cut -f1 -d'~'`
chain2=`echo $chains | cut -f2 -d'~'`

echo $chain1 $chain2

asa12=`echo ${pdbinp/\.pdb/\.asa}`
bury12=`echo ${pdbinp/\.pdb/\.bury}`

$path/EXEC/naccess.bash inp1.pdb
$path/EXEC/naccess.bash inp2.pdb
$path/EXEC/naccess.bash $pdbinp

#ls inp1.asa;
#ls inp2.asa;
#ls $asa12;

`rm -f bury.scrt`;
`rm -f bury.out`;

`$path/EXEC/buryasa.exe $asa12`;
`mv bury.out $bury12`;
out=`$path/EXEC/delasa.exe inp1.asa inp2.asa $asa12`;

#ls fort.14;
#ls fort.15;
restot=`echo ${asa12/\.asa/\.bury}`;

Ntot=`wc -l $restot | cut -f1 -d' '`
sumasa=`cut -c55-62 $asa12 | awk 'BEGIN{sum=0}{sum=sum+$1}END{print sum}'`

echo "SUMASA:" $sumasa
bsa=`echo $out | cut -f4 -d' '`
echo "BSA: " $bsa

#bc -l <<< `echo ${bsa}/${sumasa}`

nbsa=`echo ${bsa}/${sumasa} | bc -l`
echo "NBSA: " $nbsa

#exit
cp $pdbinp orig.pdb
$path/EXEC/pdb2resWMchain.fast.pl orig.pdb > /dev/null
cp orig.res inp12map.res

$path/EXEC/crmap.pl

l1=`wc -l fort.14 | cut -f1 -d' '`;
l2=`wc -l fort.15 | cut -f1 -d' '`;

echo $l1 $l2

if [ -e "fort.14" ] && [ "$l1" > "0" ]; then 
    mv fort.14 intf1.delasa
fi

if [ -e "fort.15" ] && [ "$l2" > "0" ]; then 
    mv fort.15 intf2.delasa
fi

rm -f intf1.res
rm -f intf2.res
$path/EXEC/intres.pl intf1.delasa intf2.delasa
#wc intf1.res
#wc intf2.res
#	print "$bsa      $sum_asa\n";
ifs1=`wc -l intf1.res | cut -f1 -d' '`
ifs2=`wc -l intf2.res | cut -f1 -d' '`
echo $ifs1
echo $ifs2
intfres=`echo ${ifs1}+${ifs2} | bc -l` 
echo $intfres

if [ "$intfres" > "0" ]; then        	
    #echo $intfres	$Ntot
    Fintres=`echo ${intfres}/${Ntot} | bc -l`
    echo "FINTRES: " $Fintres
else
    Fintres=0.000
fi

echo $nbsa  $Fintres > temp.$$
awk '{printf "%f %f\n",$1,$2}' temp.$$ > $outfile
rm temp.$$
cat $outfile

