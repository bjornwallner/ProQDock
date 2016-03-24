#!/bin/bash -x

#DB="/proj/wallner/users/x_bjowa/github/Rosetta/main/database"
#path="/proj/wallner/users/x_bjowa/github/Rosetta/main/source/bin/"

model=$1
native=$2
rpath=$3
rDB=$4
score=`echo $native | cut -f1 -d'.'`.rosetta_repacked
$rpath/score_jd2.linuxgccrelease -database $rDB -in:file:s $model -in:file:native $native -out:file:scorefile $score -evaluation:DockMetrics


