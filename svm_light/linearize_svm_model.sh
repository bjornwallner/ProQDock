#!/bin/bash

model=$1
modelout=$model.linear


/proj/wallner/users/x_bjowa/local/svm_light/svm_classify /dev/null $model /dev/null | grep LINEAR_WEIGHTS > /tmp/svm.weights.$$
head -n 11 $model |sed s/[0-9]\ #\ kernel\ type/4\ #\ kernel\ type/ > /tmp/svm.head.$$

cat /tmp/svm.head.$$ /tmp/svm.weights.$$ > $modelout

echo "Output written to $modelout"
rm -f /tmp/svm.head.$$
rm -f /tmp/svm.weights.$$