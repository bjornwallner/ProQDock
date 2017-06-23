#!/bin/bash -f

upath=`echo ${0/\/MAINEXEC\/runEC.bash/}`
echo $upath

path=`readlink -f $upath`
echo $path

rm -f amber.crg 
rm -f amber_dummy.siz

ln -s $path/LIBR/amber.crg .
ln -s $path/LIBR/amber_dummy.siz .

pdbinp=$1
echo $pdbinp

delphi_path=$2
echo $delphi_path

ESpath=$3
echo $ESpath

gauss=$4
echo $gauss 

if [ "$#" -lt "2" ]; then
echo "delphi_path is missing"
exit;
elif [ "$#" -lt "3" ]; then
echo "delphi_path and ESpath are missing"
exit;
elif [ "$#" == "3" ]; then
echo "The Multidielectric Gaussian flag is set to off mode"
echo "Since there were no user-input provided for this parameter"
echo "Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed"
gauss=0
elif [ "$#" == "4" ]; then
	if [ "$gauss" == "1" ]; then
	echo "The Multidielectric Gaussian flag is up"
	echo "Delphi will commense with its Multidielctric Gaussian feature"
	elif [ "$gauss" == "0" ]; then
	echo "The Multidielectric Gaussian flag is down"
	echo "Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed"
	fi
fi

        rm -f chfn.inp
        echo $pdbinp > chfn.inp
        fnch=`$path/EXEC/chfn.pl`
	echo $fnch
	if [ `echo $fnch` == "OK" ]; then 
	echo "You can proceed"
	else
	exit;
	fi

	echo "Here I am"

#============================================================================================================================

echo 'You entered:' $pdbinp
code=`echo $pdbinp | cut -f1 -d"."`

#=======================================================================
#  CHECK INPUT PDB FORMAT AND OTHER REQUIREMENTS 
#  ACCEPT OR REJECT
#=======================================================================

#=======================================================================
# Convert to original Reduce format (e.g., "HG12" -> "2HG1")  if necessary (otherwise skip)
# The 13th column should be either blank or a numeric and never the atom type "H" 
# Rather the 14th column should contain the atom type "H" 
#=======================================================================

$path/EXEC/reducemap.pl $pdbinp > temp.pdb
mv temp.pdb $pdbinp

#awk '$1=="ATOM" || $1=="HETATM"' formp.pdb > temp.pdb
cp $pdbinp temp.pdb
$path/EXEC/metRrename.csh temp.pdb
mv temp.pdb $pdbinp

#=========================================
# REFRESH DIRECTORY (remove old files if present) 
#=========================================
rm -f alter.inp
rm -f surf_dot
rm -f surf_out
rm -f surf.pdb
rm -f check
rm -f sg.out
rm -f sg.con
rm -f icon.out
rm -f num.out
rm -f remove.out
rm -f inp.pdb
rm -f sphere.dot
rm -f sucal1.out
rm -f out.satv
rm -f dsl.out
rm -f dsl.num
rm -f *.log
rm -f *.pot
rm -f *cout.pdb
rm -f *sout.pdb
rm -f inp*
rm -f dsl.num
rm -f dsl.out
rm -f ARCDAT
rm -f core
rm -f ARCDAT
rm -f fort*
rm -f outhiscysO.pdb
rm -f outhisO.pdb
rm -f hiscysNC.pdb
rm -f msph.dot
rm -f numM.res
rm -f met.cores
rm -f fpp.status
rm -f formp.pdb
rm -f formch.out
rm -f redun.out
rm -f res1.out
rm -f temp*
rm -f target.res
rm -f res.replace
rm -f script1.prm
rm -f script2.prm
rm -f chfn.inp
rm -f sres.res
rm -f smres.res
rm -f smresE.res
#=========================================

rm -f redun.out
rm -f rdn.log

cp $pdbinp orig.pdb
NO=`$path/EXEC/pdb2resWMchain.fast.pl orig.pdb`
#wc -l orig.res

$path/EXEC/mapresno1to1.pl $pdbinp > mapped.pdb
$path/EXEC/metRrename.csh mapped.pdb 
mv mapped.pdb $pdbinp

chains=`$path/EXEC/chainsplit.pl $pdbinp`
chain1=`echo $chains | cut -f1 -d'~'`
chain2=`echo $chains | cut -f2 -d'~'`

echo "TWO POLYPEPTIDE CHAINS FOUND IN THE INPUT MOLECULE: CHAIN" \'$chain1\' "AND CHAIN" \'$chain2\'

code=`echo $pdbinp | cut -f1 -d'.'`
#echo $code

pdb1=`echo $code`_`echo $chain1`.pdb
pdb2=`echo $code`_`echo $chain2`.pdb
pdb12=`echo $code`_`echo $chain1``echo $chain2`.pdb

echo "PDB FILE CONTAINING THE FIRST CHAIN :" $pdb1
echo "PDB FILE CONTAINING THE SECOND CHAIN:" $pdb2
echo "PDB FILE CONTAINING BOTH THE CHAINS :" $pdb12

cp inp1.pdb $pdb1
cp inp2.pdb $pdb2
cp $pdbinp $pdb12

#=================================================================
#  MAP PDB FROM RES 1 (ALREADY MAPPED)
#=================================================================
echo "==============================================================="

cp $pdb1 inp1map.pdb
N1=`$path/EXEC/pdb2resWMchain.fast.pl inp1map.pdb`
echo "$pdb1 CONTAINS $N1 RESIDUES"
cp $pdb2 inp2map.pdb
N2=`$path/EXEC/pdb2resWMchain.fast.pl inp2map.pdb`
echo "$pdb2 CONTAINS $N2 RESIDUES"
cp $pdb12 inp12map.pdb
N3=`$path/EXEC/pdb2resWMchain.fast.pl inp12map.pdb`
echo "$pdb12 CONTAINS $N3 RESIDUES"

echo "==============================================================="

lench=`wc -l inp12map.res | cut -f1 -d' '`

echo "==============================================================="
echo "CHECKING CAHIN-LENGTH"
echo "==============================================================="


if [ "$lench" -gt "2500" ] ; then 
echo "============================================================="
echo "Your PDB file contains "$lench" residues"
echo "Program will exit"
echo "PDB file should contain not more than 2500 residues"
echo "============================================================="
exit;
else 
echo "============================================================="
echo "Length of the polypeptide chain is $lench (which is OK)"
echo "Program will proceed"
echo "============================================================="
fi

#exit;

#goto SKIP

#PROCEED1:

echo "==============================================================="
echo "SETTING FIELDS"
echo "==============================================================="

asa1=`echo $pdb1 | cut -f1 -d '.'`.asa
asa2=`echo $pdb2 | cut -f1 -d '.'`.asa
asa12=`echo $pdb12 | cut -f1 -d '.'`.asa
asa1log=`echo $pdb1 | cut -f1 -d '.'`.asalog
asa2log=`echo $pdb2 | cut -f1 -d '.'`.asalog
asa12log=`echo $pdb12 | cut -f1 -d '.'`.asalog
dslf=`echo $code`.dsl
Smfile=`echo $code`.Sm
Emfile=`echo $code`.Em
CSplot=`echo $code`.CSplot

surf1=`echo $pdb1 | cut -f1 -d '.'`-surf.pdb
surf2=`echo $pdb2 | cut -f1 -d '.'`-surf.pdb

dsurf1=`echo $pdb1 | cut -f1 -d '.'`.dsurf
dsurf2=`echo $pdb2 | cut -f1 -d '.'`.dsurf

intf1=`echo $pdb1 | cut -f1 -d '.'`-intf.res
intf2=`echo $pdb2 | cut -f1 -d '.'`-intf.res

surf1forsatv=`echo $pdb1 | cut -f1 -d '.'`.surf
surf2forsatv=`echo $pdb2 | cut -f1 -d '.'`.surf
surf12forsatv=`echo $pdb12 | cut -f1 -d '.'`.surf

int1surf=`echo $pdb1 | cut -f1 -d '.'`.isurf
int2surf=`echo $pdb2 | cut -f1 -d '.'`.isurf

int1spdb=`echo $pdb1 | cut -f1 -d '.'`-isurf.pdb
int2spdb=`echo $pdb2 | cut -f1 -d '.'`-isurf.pdb

iatom1=`echo $pdb1 | cut -f1 -d'.'`.iatom
iatom2=`echo $pdb2 | cut -f1 -d'.'`.iatom

rasscript=`echo $code`-intsurf.spt
metcores=`echo $code`.mco
metrasview=`echo $code`-met.spt
surf=`echo $code`-surf.pdb
#outScEC=`echo $code`.ScEC
outEC=`echo $code`.EC

#=========================================================================
#  CALCULATE SOLVENT ACCESSIBLE SURFACE AREA AND DETERMINE THE INTERFACE
#  INTERFACIAL ATOMS SHOULD HAVE DELSAA = (SAA(FREE) - SAA(BOUND)) != 0 
#  GENERATE INTERFACE RES FILES
#=========================================================================

echo "==============================================================="
echo "CALCULATING SOLVENT ACCESSIBLE AREAS AND DETTERMINING INTERFACE"
echo "==============================================================="

rm met.cores
rm met.rasview
$path/EXEC/mcoord.exe $pdb12
Nmet=`wc -l met.cores | cut -f1 -d' '`
	if [ "$Nmet" == "0" ]; then
	echo "============================================================"
	echo "============================================================"
	echo "               NO METAL CO-ORDINATION DETECTED"
	echo "============================================================"
	echo "============================================================"
	else
	mv met.cores $metcores
	mv met.rasview $metrasview
	cat $metcores
#	rasmol -script $metrasview &
	fi

rescut=`$path/EXEC/rescut.pl $pdbinp`
#echo $rescut

$path/EXEC/naccess.bash $pdb1	# A.pdb (original)
$path/EXEC/naccess.bash $pdb2	# B.pdb (original)
$path/EXEC/naccess.bash $pdb12	# AB.pdb (original)

$path/EXEC/delasa.exe $asa1 $asa2 $asa12 > temp
Nint=`awk '{print $1}' temp`

echo "NUMBER OF INTERFACIAL ATOMS BURIED UPON COMPLEXATION:" $Nint

rm -f check.Nint

echo $Nint

if [ "$Nint" == "0" ]; then
echo "================================================================================================================="
echo "================================================================================================================="
echo "================================================================================================================="
echo "NO INTERFACIAL ATOMS DETECTED. THE CHAINS ARE NON-INTERACTING AS IT SEEMS. HAVE A LOOK AT THE PDB FILE in RASMOL"
echo "NO_Nint" > check.Nint
echo "$code.pdb: NO INTERFACIAL ATOMS DETECTED. THE CHAINS ARE NON-INTERACTING AS IT SEEMS. HAVE A LOOK AT THE PDB FILE IN RASMOL / PYMOL / CHIMERA " > $code.log
echo "EC set to -1.00 AS A PENALTY"
EC=`echo -1.00`
echo $EC > $outEC
echo "================================================================================================================="
echo "================================================================================================================="
echo "================================================================================================================="
exit
fi

#outfile: fort.14, fort.15, fort.16

cp fort.14 $iatom1
cp fort.15 $iatom2

echo "=============================================================="
echo "=============================================================="
cat fort.16
cp fort.16 $code.asaAngsq
echo "=============================================================="
echo "=============================================================="

rm ras*.select

#wc -l inp12map.res
#wc -l orig.res

$path/EXEC/crmap.pl
$path/EXEC/intres.pl $iatom1 $iatom2
#outfile: intf1.res, intf2.res, ras.select

mv intf1.res $intf1
mv intf2.res $intf2

#=========================================================================
# CALCULATE BURIAL OF RESIDUES AS IN THE WHOLE COMPLEX
#=========================================================================

echo "==============================================================="
echo "CALCULATING BURIAL OF RESIDUES AS IN THE WHOLE (COMPLEX) MOLECULE"
echo "==============================================================="


#=========================================================================
# GENERATE SURFACE FOR MOLECULE 1 
#=========================================================================

#==================================================================================================
#======================================= SURFACE FILE FOR INTERFACE 1 =============================
#==================================================================================================

$ESpath/EDTSurf -i $pdb1 -s 3 -f 1 -o out
mv out.ply.surf $dsurf1
$ESpath/EDTSurf -i $pdb2 -s 3 -f 1 -o out 
mv out.ply.surf $dsurf2

echo "==========================================================================================="
echo $dsurf1
echo $dsurf2
echo "==========================================================================================="

echo "==========================================================================================="
echo $iatom1
echo $iatom2
echo "==========================================================================================="

$path/EXEC/fetchds.exe $iatom1 $dsurf1
mv fort.28 intsurf1.pdb
mv fort.38 outms1-surf.pdb
$path/EXEC/fetchds.exe $iatom2 $dsurf2
mv fort.28 intsurf2.pdb
mv fort.38 outms2-surf.pdb

#cat intsurf1.pdb intsurf2.pdb > intsurf12.pdb
#cat outms1-surf.pdb outms2-surf.pdb > outms12-surf.pdb
#wc intsurf12.pdb
#wc outms12-surf.pdb

#./scriptmodinp.bash $pdb1 $pdb2

#inp1=$1
#inp2=$2

$path/EXEC/pdb2resWMchain.fast.pl $pdb1
$path/EXEC/pdb2resWMchain.fast.pl $pdb2

rm dsl.num
rm dsl.out
$path/EXEC/dsl.exe $pdb1
mv dsl.out inp.dsl
rm dsl.num
$path/EXEC/his2hidep.pl $pdb1	# internally calls cys2cyxs.pl
echo "Calling $path/EXEC/cys2cyxs.pl for CYS -> CYX conversion"
$path/EXEC/cys2cyxs.pl
$path/EXEC/ntrename.pl
mv hiscysNC.pdb inp1m.pdb

$path/EXEC/dsl.exe $pdb2
mv dsl.out inp.dsl
rm dsl.num
$path/EXEC/his2hidep.pl $pdb2	# internally calls cys2cyxs.pl
echo "Calling $path/EXEC/cys2cyxs.pl for CYS -> CYX conversion"
$path/EXEC/cys2cyxs.pl
$path/EXEC/ntrename.pl
mv hiscysNC.pdb inp2m.pdb

$path/EXEC/makedumpdbint2.pl inp1m.pdb inp2m.pdb

wc real1dum2.pdb
wc real2dum1.pdb

echo "DELPHI INPUTS CREATED WITH DUMMY ATOMS"

#exit;

# intsurf1.pdb and intsurf2.pdb should be pre-existing 
#

#SKIP: 

gsz=`$path/EXEC/hdist.exe $pdbinp`

if [ "$lench" -gt "1000" ]; then 
echo "length exceeds 1000 : " $lench
gsz=281
fi

echo "GRID SIZE FOR DELPHI: " $gsz


$path/EXEC/generateprm26.pl $path real1dum2.pdb intsurf1.pdb outmod1.pdb outsurf11.pot $gsz $gauss
echo "$path/EXEC/generateprm26.pl $path real1dum2.pdb intsurf1.pdb outmod1.pdb outsurf11.pot $gsz $gauss"
mv script.prm script11.prm 
$path/EXEC/generateprm26.pl $path real2dum1.pdb intsurf1.pdb outmod2pdb outsurf21.pot $gsz $gauss
echo "$path/EXEC/generateprm26.pl $path real2dum1.pdb intsurf1.pdb outmod2pdb outsurf21.pot $gsz $gauss"
mv script.prm script21.prm 

echo "delphi is running now from $delphi_path"

#exit;

if [ "$gauss" == "0" ]; then
	rm ARCDAT
	$delphi_path/delphi95 script11.prm > log11
	rm ARCDAT
	$delphi_path/delphi95 script21.prm > log21
elif [ "$gauss" == "1" ]; then
	rm ARCDAT
	$delphi_path/delphi95 script11.prm > log11
	rm ARCDAT
	$delphi_path/delphi95 script21.prm > log21
fi

$path/EXEC/extpot.pl outsurf11.pot > temp11.pot
$path/EXEC/extpot.pl outsurf21.pot > temp21.pot

$path/EXEC/generateprm26.pl $path real1dum2.pdb intsurf2.pdb outmod1.pdb outsurf12.pot $gsz $gauss
echo "$path/EXEC/generateprm26.pl $path real1dum2.pdb intsurf2.pdb outmod1.pdb outsurf12.pot $gsz $gauss"
mv script.prm script12.prm 
$path/EXEC/generateprm26.pl $path real2dum1.pdb intsurf2.pdb outmod2pdb outsurf22.pot $gsz $gauss
echo "$path/EXEC/generateprm26.pl $path real2dum1.pdb intsurf2.pdb outmod2pdb outsurf22.pot $gsz $gauss"
mv script.prm script22.prm 

if [ "$gauss" == "0" ]; then
	rm ARCDAT
	$delphi_path/delphi95 script12.prm > log12
	rm ARCDAT
	$delphi_path/delphi95 script22.prm > log22
elif [ "$gauss" == "1" ]; then
	rm ARCDAT
	$delphi_path/delphi95 script12.prm > log12
	rm ARCDAT
	$delphi_path/delphi95 script22.prm > log22
fi

$path/EXEC/extpot.pl outsurf12.pot > temp12.pot
$path/EXEC/extpot.pl outsurf22.pot > temp22.pot

i1=`$path/EXEC/ccpsw.exe temp11.pot temp21.pot`
echo $i1
i2=`$path/EXEC/ccpsw.exe temp12.pot temp22.pot`
echo $i2

flagEC=0

if [ "$i1" == "NaN" ] && [ "$i2" != "NaN" ]; then 
EC=$i2
flagEC=1
fi

if [ "$i2" == "NaN" ] && [ "$i1" != "NaN" ]; then
EC=$i1
flagEC=1
fi

if [ "$i2" == "NaN" ] && [ "$i1" == "NaN" ]; then
EC=-1.00
flagEC=1
fi

echo $flagEC

if [ "$flagEC" == "1" ]; then
echo "EC=" $EC
elif [ "$flagEC" == "0" ]; then 
EC=`$path/EXEC/mean.pl $i1 $i2`
echo "EC=" $EC
fi

echo $EC > $outEC
cat $outEC
# CLEAN

rm -f orig.pdb
rm -f orig.res
rm -f res1.out
rm -f temp2
rm -f temp1
rm -f inp1.pdb
rm -f inp2.pdb
rm -f inp1map.pdb
rm -f inp1map.res
rm -f inp2map.pdb
rm -f inp2map.res
rm -f inp12map.pdb
rm -f inp12map.res
rm -f met.cores
rm -f met.rasview
rm -f fort.14
rm -f fort.15
rm -f fort.16
rm -f reso2n.map
rm -f ras1.select
rm -f ras2.select
rm -f pdbwH.pdb
rm -f out-cav.pdb
rm -f out.ply
rm -f out.ply.pdb
rm -f outms1-surf.pdb
rm -f intsurf1.pdb
rm -f outms2-surf.pdb
rm -f intsurf2.pdb
rm -f intsurf12.pdb
rm -f outms12-surf.pdb
rm -f redun.out
rm -f rdn.log
rm -f inp1m.pdb
rm -f inp.dsl
rm -f outhisO.pdb
rm -f outhiscysO.pdb
rm -f inp2m.pdb
rm -f real1dum2.pdb
rm -f real2dum1.pdb
rm -f ARCDAT
rm -f outsurf11.pot
rm -f log11
rm -f outsurf21.pot
rm -f log21
rm -f temp11.pot
rm -f temp21.pot
rm -f script11.prm
rm -f script21.prm
rm -f script12.prm
rm -f script22.prm
rm -f outmod1.pdb
rm -f outsurf12.pot
rm -f log12
rm -f outmod2pdb
rm -f outsurf22.pot
rm -f log22
rm -f temp12.pot
rm -f temp22.pot
rm -f fort.777
rm -f fort.888
rm -f fort.999




