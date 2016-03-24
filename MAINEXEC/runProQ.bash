#!/bin/bash

pdb=$1
proqpath=$2
proqscorepath=$3
nseqflag=$4   # 1 (single pdb), 2 (multi pdb)

if [ "$nseqflag" == "1" ]; then 
$proqpath/run_all_external.pl -overwrite 1 -cpu 4 -pdb $pdb;
elif [ "$nseqflag" == "2" ]; then 
    if [ -e "fulllength.seq" ]; then 
	cat fulllength.seq;		
	lenc1=`wc -l $c1 | cut -f1 -d' '`;
        if [ $lenc1 == "0" ]; then        
        echo "fulllength.seq is a blank file\n";
        echo "It would be deleted and merge_all.pl will rerun\n";
        rm -f fulllength.seq*;
        $proqpath/merge_all.pl;
	$proqpath/run_all_external.pl -overwrite 1 -cpu 4 -fasta fulllength.seq;
        else        
        echo "fulllength.seq exists\n";
        echo "merge_all.pl will be skipped\n"; 
	$proqpath/run_all_external.pl -overwrite 1 -cpu 4 -fasta fulllength.seq;
	fi
    else
	echo "fulllength.seq does not exist";
	echo "merge_all.pl will run";
	$proqpath/merge_all.pl;
	$proqpath/run_all_external.pl -overwrite 1 -cpu 4 -fasta fulllength.seq;
    fi
fi

#=================================== ProQ2 =============================================

lch=`wc -l $pdb.acc | cut -f1 -d' '`
echo $lch

if [ -e "$pdb.acc" ]; then
echo "==============================================="
echo $pdb.acc FOUND
echo "==============================================="
        if [ "$lch" == "1" ]; then
        echo "==============================================="
        echo $pdb.acc IS A NON-EMPTY FILE
        echo PROGRAM WILL RUN
	rm -f $pdb.ProQ2
        echo "==============================================="
        $proqscorepath/score.linuxgccrelease -in:file:s  $pdb -ProQ:basename $pdb -score:weights ProQ2 -out:file:scorefile $pdb.ProQ2 -run:nodelay
        else
        echo "==============================================="
        echo $pdb.acc IS A EMPTY FILE
        echo "==============================================="
        fi
else
echo "==============================================="
echo $pdb.acc NOT FOUND
echo "==============================================="
fi





