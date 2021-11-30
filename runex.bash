#!/bin/bash

#for i in `ls ../ProQDock/EXAMPLE_CASE_STUDY/D1OYV*-delpatch.pdb`
for i in EXAMPLE_CASE_STUDY/D1OYV*-delpatch.pdb
do
    echo "./run_ProQDock.py $i EXAMPLE_CASE_STUDY/1OYV.pdb.fasta"
    ./run_ProQDock.py $i EXAMPLE_CASE_STUDY/1OYV.pdb.fasta"
done

