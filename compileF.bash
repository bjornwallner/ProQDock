#!/bin/bash

upath=`echo ${0/compileF.bash/}`
path=`readlink -f $upath`

fortcomp=$1

if [ "$#" == "0" ]; then 
echo "Fortran90 compiler not specified"
echo "Default fortran 90 compiler: ifort"
fortcomp=ifort
fi

chs=`which $fortcomp`
echo $chs

if [[ $chs == *"$fortcomp"* ]]; then
echo "$fortcomp FOUND"
else
echo "$fortcomp NOT FOUND to be installed"
exit;
fi

$fortcomp $path/EXEC/globuleN.f -o $path/EXEC/glbN.exe &> /dev/null
$fortcomp $path/EXEC/metsph.f -o $path/EXEC/msph.exe &> /dev/null
$fortcomp $path/EXEC/intcont6.f -o $path/EXEC/intcont6.exe &> /dev/null
$fortcomp $path/EXEC/accall.f -o $path/EXEC/accall.exe &> /dev/null
$fortcomp $path/EXEC/delasa.f -o $path/EXEC/delasa.exe &> /dev/null
$fortcomp $path/EXEC/hdist.f -o $path/EXEC/hdist.exe &> /dev/null
$fortcomp $path/EXEC/buryasa.f -o $path/EXEC/buryasa.exe &> /dev/null
$fortcomp $path/EXEC/intcont.f -o $path/EXEC/intcont.exe &> /dev/null
$fortcomp $path/EXEC/fetchintsurf.f -o $path/EXEC/fetchds.exe &> /dev/null
$fortcomp $path/EXEC/corrcoefPsplw.f -o $path/EXEC/ccpsw.exe &> /dev/null
$fortcomp $path/EXEC/metcoord.f -o $path/EXEC/mcoord.exe &> /dev/null
$fortcomp $path/EXEC/contpref20CB.f -o $path/EXEC/contpref20CB.exe &> /dev/null
$fortcomp $path/EXEC/linkdenN.f -o $path/MAINEXEC/ldN.exe &> /dev/null
$fortcomp $path/EXEC/resGbur.f -o $path/EXEC/rGb.exe &> /dev/null
$fortcomp $path/EXEC/intf2surf.f -o $path/EXEC/isurf.exe &> /dev/null
$fortcomp $path/EXEC/dsl.f -o $path/EXEC/dsl.exe &> /dev/null


Nfort=`ls $path/EXEC/*.f | wc -l`
Nfort=`echo ${Nfort}-1 | bc -l` 

Nexec=`ls $path/EXEC/*.exe | wc -l`
echo $Nexec

if [ "$Nexec" == "$Nfort" ] && [ -e "$path/MAINEXEC/ldN.exe" ]; then
echo "Compilation Successful"
fi

