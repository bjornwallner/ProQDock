<<<<<<< HEAD
# ProQDock

Requires PERL (v.5.8 or higher), and a fortran90 compiler (prefered: ifort) 
and five additional packages to be pre-installed 

1. sc from ccp4 () [executable name: sc] 
2. delphi v.4 or higher () [executable name: delphi_static / delphi95] 
3. EDTSurf ()
4. Rosetta () 
5. ProQ2 ()

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

`proqscorepath=/proj/wallner/apps/rosetta/current/main/source/bin`

Run with
`./ProQDock.bash <model.PDB>`  The input pdb file should have an extension .pdb / .PDB 
Run from any Directory with the short / full path of the ProQDock/ProQDock.bash
`optional arguments: -g (0/1)` delphi_mode: 0: single_dielectric; 1: multidielctric mode

Example

bash$ `./ProQDock.bash ./TESTPDBS/1u07.pdb`

==============================================================================================
Your Input: 1u07.pdb
The file has a Proper extension (.pdb / .PDB)
The Multidielectric Gaussian flag is set to 'off' mode
No user-input provided for this parameter
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
total 260
-rw-rw-r-- 1 x_sabas x_sabas    130 Mar 24 17:07 1u07.Rterms
-rw-rw-r-- 1 x_sabas x_sabas    189 Mar 24 17:07 1u07.pdb.fasta
-rw-rw-r-- 1 x_sabas x_sabas    272 Mar 24 17:09 1u07.SVMfeatures
-rw-rw-r-- 1 x_sabas x_sabas     53 Mar 24 17:09 1u07.SVMfeatures.pred5
-rw-rw-r-- 1 x_sabas x_sabas     11 Mar 24 17:09 1u07.ProQDock
drwx------ 3 x_sabas x_sabas   4096 Mar 24 17:09 ..
-rw-rw-r-- 1 x_sabas x_sabas 234150 Mar 24 17:09 1u07.pdb
drwxrwxr-x 2 x_sabas x_sabas   4096 Mar 24 17:09 .
-rw-rw-r-- 1 x_sabas pg_wallner 11 Mar 24 17:09 OUT1u07/1u07.ProQDock
   0.85360
OUTPUT Directory OUT1u07
Features:  OUT1u07/1u07.SVMfeatures
ProQDock score:  OUT1u07/1u07.ProQDock
total 64
drwxrwsr-x 18 x_sabas pg_wallner  32768 Mar 24 16:59 ..
-rw-rw-r--  1 x_sabas pg_wallner    272 Mar 24 17:09 1u07.SVMfeatures
-rw-rw-r--  1 x_sabas pg_wallner     53 Mar 24 17:09 1u07.SVMfeatures.pred5
-rw-rw-r--  1 x_sabas pg_wallner    189 Mar 24 17:09 1u07.pdb.fasta
-rw-rw-r--  1 x_sabas pg_wallner     11 Mar 24 17:09 1u07.ProQDock
-rw-rw-r--  1 x_sabas pg_wallner    130 Mar 24 17:09 1u07.Rterms
drwxrwsr-x  2 x_sabas pg_wallner   4096 Mar 24 17:09 .
-rw-rw-r--  1 x_sabas pg_wallner 234150 Mar 24 17:09 1u07.pdb

=======
# ProQDock
>>>>>>> 014d763b1107b208f5ee92f154ea34b1df717313
