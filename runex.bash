#!/bin/bash

for i in `ls ../ProQDock/EXAMPLE_CASE_STUDY/D1OYV*-delpatch.pdb`
do
echo "./ProQDock.bash $i ./EXAMPLE_CASE_STUDY/1OYV.pdb.fasta"
./ProQDock.bash $i ./EXAMPLE_CASE_STUDY/1OYV.pdb.fasta
done

