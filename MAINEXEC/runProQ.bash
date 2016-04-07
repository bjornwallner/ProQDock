#!/bin/bash

pdb=$1
proqpath=$2
proqscorepath=$3
fulllength_fasta=$4

if [ ! -s "$fulllength_fasta.acc" ]; then
    rm -f $fulllength_fasta.acc
fi

if [ ! -e "$fulllength_fasta.acc" ]; then
    $proqpath/run_all_external.pl -cpu 4 -fasta $fulllength_fasta;
fi


if hash needle 2>/dev/null; then
    echo "FOUND: needle..."
    $proqpath/copy_features_from_master.pl $pdb $fulllength_fasta 
    $proqscorepath/score.linuxgccrelease -in:file:s  $pdb -ProQ:basename $pdb -score:weights ProQ2 -out:file:scorefile $pdb.ProQ2 -run:nodelay
else
    echo "Assuming pdb seq is equal to $fulllength_fasta otherwise it will crash...."
    echo "Install needle from the EMBOSS package and the program will work... maybe"
    $proqscorepath/score.linuxgccrelease -in:file:s  $pdb -ProQ:basename $fulllength_fasta -score:weights ProQ2 -out:file:scorefile $pdb.ProQ2 -run:nodelay
fi

exit

