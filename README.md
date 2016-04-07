# ProQDock

Requires PERL (v.5.8 or higher), and a fortran90 compiler (prefered: ifort) 
and five additional packages to be pre-installed 

1. sc from ccp4 (http://www.ccp4.ac.uk/) [executable_name: sc] 
2. delphi v.4 or higher (http://compbio.clemson.edu/delphi/) [executable_name: delphi_static / delphi95] 
3. EDTSurf (http://zhanglab.ccmb.med.umich.edu/EDTSurf/) [executable_name: EDTSurf]
4. Rosetta (https://www.rosettacommons.org/software/)  
5. ProQ2 (https://github.com/bjornwallner/ProQ_scripts/)
6. needle from EMBOS (http://emboss.sourceforge.net/download/)

Installation

`git clone https://github.com/nemo8130/ProQDock`

`cd ProQDock`

`./install.bash <fortran90-compiler>`  (Default: ifort)

open ProQDock.bash in any editor and change the paths for the following fields according to your preference

`sc_path=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin`

`delphi_path=/home/x_sabas/bin`

`rosetta_path=/proj/wallner/users/x_bjowa/github/Rosetta/main/source/bin`

`rosetta_db=/proj/wallner/users/x_bjowa/github/Rosetta/main/database`

`svm_path=/home/x_sabas/proj/svm_light`

`ESpath=/home/x_sabas/proj/EDTSurf`

`proqpath=/home/x_sabas/proj/BACKUP/ProQ_scripts/bin`

The program essentially has two inputs : 

	1. The coordinate (PDB) file for the model
	2. The full-length target sequence

As a Preparatory Step, The fulllength (FASTA) target sequence (one letter amino acid code) should be prepared and provided as an apriori alongside the coordinate (PDB) file.

You can generate the fasta sequence by using: 

./ProQDock/EXEC/pdb2fasta.pl <input.pdb>  >  input.pdb.fasta

Run with
`./ProQDock.bash <model.PDB> <fulllength.fasta>`  model.pdb: The input pdb file fulllength.fasta: The fulllength fasta sequence
Run from any Directory with the short / full path of the ProQDock/ProQDock.bash
`optional arguments: -g (0/1)` delphi_mode: 0: single_dielectric; 1: multidielctric mode


If you want to run it for a list of PDB files (models) corresponding to the same target sequence, 
then SKIP repeating the profile generation step more than once, by providing a consistent basename for the fasta file. 

Launch the `./runex.bash` script which runs on a set of (example) models kept at EXAMPLE_CASE_STUDY/

=============================================================================================
`#!/bin/bash`

for i in `ls ../ProQDock/EXAMPLE_CASE_STUDY/D1OYV*-delpatch.pdb`
do
echo "./ProQDock.bash $i ./EXAMPLE_CASE_STUDY/1OYV.pdb.fasta"
./ProQDock.bash $i ./EXAMPLE_CASE_STUDY/1OYV.pdb.fasta
done

===================================== OUTPUT ================================================
`cat EXAMPLE_CASE_STUDY/D1OYV-models.status.results`
=============================================================================================

`#           Model         fnat       LRMS       iRMS     CAPRI_class    DockQ    ProQDock`
`         D1OYV-a102a      0.000     35.348     15.846     Incorrect    0.02118    0.05908`
`         D1OYV-a104c      0.000     44.703     16.626     Incorrect    0.01432    0.21024`
`         D1OYV-a106c      0.000     44.600     18.136     Incorrect    0.01395    0.03890`
`         D1OYV-a115c      0.000     31.946     11.941     Incorrect    0.02722    0.02730`
`          D1OYV-a61c      0.652      6.820      2.334    Acceptable    0.51761    0.51060`
`          D1OYV-a61d      0.630      6.574      2.450    Acceptable    0.50960    0.46046`
`          D1OYV-a81d      0.587      6.788      2.481    Acceptable    0.48841    0.42648`
`          D1OYV-a41b      0.717      3.619      1.428        Medium    0.69617    0.48751`
`          D1OYV-a62c      0.707      3.991      1.628        Medium    0.66168    0.55795`
`          D1OYV-a82d      0.707      4.347      1.593        Medium    0.65639    0.48664`

=============================================================================================
Example: 
=============================================================================================

bash$ `./ProQDock.bash ./TESTPDBS/1u07.pdb ./EXAMPLE/1u07.pdb.fasta`
==============================================================================================
Your Input: 1u07.pdb
The file has a Proper extension (.pdb / .PDB)
Normal Delphi (single internal dielectric, epsilon = 2.0) will be executed
===========================================================
Total number of residues in the Complex: 178

EC=0.11
Sc=0.767
rGb=0.07170
Ld= 0.14815
nBSA=0.11534
Fintres=0.28652
CPscore= 0.48131
CPM= 0.89454
rTs=0.08808
Isc=0.00002
Erep=0.27537
Etmr=0.21025
ProQ= 0.845

================The ProQDock Score ====================

   0.85360

=======================================================
ProQDock: A Protein-Protein Docking Scoring Function using high level coordinate driven features and Machine Learning

usage: $exec_path/ProQDock.bash $pdbpath/input.pdb -g (0/1)
-g is an optional argument which determines the Delphi mode to be executed (0: Single-Dielectric / 1: Multi-Dielectric) [Default: 0] 

======================================================================================================================================================================
Feature   Range      Favorable Directions       Description                                                   Feature_Type    Stability*     Native_values (DB3)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
EC:       [-1,1]     Higher the Better       Electrostatic balance at the interface                             Interface      Variable      0.20 (+/-0.38)
Sc:       [-1,1]     Higher the Better       Geometric / Steric fit at the interface                            Interface       Stable	     0.68 (+/-0.17)
rGb:      [-1,1]     Higher the Better       Distribution of hydrophobicity with respect to burial              All_Atom        Stable	     0.06 (+/-0.02)
Ld:       [0, 1]         Ambiguous           Link (packing) densely at the interface                            Interface       Stable	     0.15 (+/-0.17)
nBSA:     [0, 1]         Ambiguous           Size of the interface relative to the whole complex                Interface      Variable      0.07 (+/-0.05)
Fintres:  [0, 1]         Ambiguous           Size of the interface relative to the whole complex                Interface      Variable      0.19 (+/-0.13)
CPscore:  [0, 1]     Higher the Better       Preference of different inter-residue contacts at the interface    Interface      Variable	     0.35 (+/-0.17)
CPM:      [0, 1]     Higher the Better       Likelyhood of (Sc,EC) as a function of nBSA compared to native     Interface      Variable	     0.80 (+/-0.18)
rTs:      [0, 1]     Lower the Better        Energetic Stability of the whole molecule                          All_Atom        Stable	     0.22 (+/-0.21)
Isc:      [0, 1]     Lower the Better        Binding energy                                                     Interface      Variable	     0.20 (+/-0.33)
Erep:     [0, 1]     Lower the Better        Unfavourable Energy due to atomic Clashes                          All_Atom        Stable	     0.41 (+/-0.15)
Etmr:     [0, 1]     Lower the Better        Energetic Stability without condiderating clashes                  All_Atom        Stable	     0.28 (+/-0.16)
ProQ:     [0, 1]     Higher the Better       Overall Structural Quality                                         All_Atom       Variable	     0.80 (+/-0.12)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
* In Correct Models 
======================================================================================================================================================================

ProQDock is trained over these features using Support Vectore Machines and tries to predict the DockQ score which is a continious quality measure consistant with the CAPRI evaluation protocol.

