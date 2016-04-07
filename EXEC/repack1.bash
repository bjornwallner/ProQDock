#!/bin/bash

pdb=$1
upath=`dirname $0`
echo $upath
scriptpath=`readlink -f $upath`
echo $scriptpath

path=$2
#path="/proj/wallner/users/x_bjowa/github/Rosetta/main/source/bin/"

# rebuild side-chain, backbone unchanged.
$path/relax.linuxgccrelease -in:file:s $pdb -out:prefix repack. -relax:script $scriptpath/repack.script -nstruct 1 -overwrite -run:constant_seed  

# rebuild side-chain - 1 round minimize without constraints
#$path/relax.linuxgccrelease -in:file:s $pdb -out:prefix repack_min_nocst. -relax:script ~/proj/UTILITY_PROGRAMS/repack_min.script -overwrite

# rebuild side-chain - 1 round minimize with constraints
#$path/relax.linuxgccrelease -in:file:s $pdb -out:prefix repack_min_cst. -relax:script ~/proj/UTILITY_PROGRAMS/repack_min.script -constrain_relax_to_start_coords -overwrite

# Full rounds of minimizations and side-chain rebuilding 1-10 min.
#$path/relax.linuxgccrelease -in:file:s 1ijy.pdb -constrain_relax_to_start_coords 

