# ProQDock



Requires Python 3.x, `absl-py` and `numpy`

1. sc from ccp4 (http://www.ccp4.ac.uk/) [executable_name: sc]
2. delphi v.6.2 (http://compbio.clemson.edu/delphi) [executable_name: delphi95]
3. Rosetta (https://www.rosettacommons.org/software/)
4. ProQ2 (https://github.com/bjornwallner/ProQ_scripts/)
5. needle from EMBOS (http://emboss.sourceforge.net/download/)

### Installation

```sh
$ git clone https://github.com/bjornwallner/ProQDock
$ cd ProQDock
$ ./compileF # Assumes you have gfortran installed
```
edit the `flagfile` and change the fullpaths for the following fields according to your system and save it as `myflagfile`: 

```sh
--sc_path=/software/presto/software/CCP4/7.1.016-foss-2019b-SHELX-ARP-8.0-1/ccp4-7.1/bin/sc
--delphi_path=/proj/wallner/users/x_bjowa/local/DelPhi_Linux_SP_F95/delphi95
--rosetta=/proj/wallner/apps/rosetta/Rosetta/main/
--svm_path=/proj/wallner/users/x_bjowa/local/svm_light_linux64/
--proqpath=/proj/wallner/users/x_bjowa/github/ProQ_scripts/bin/
```

## Running:

        1. The coordinate (PDB) file for the model
        2. The full-length target sequence (in FASTA format) 

- The model must be either the full-length sequence or subset of the target sequence 
- i.e., Any and all sub-sequence present in the model must be present in the tareget
- PDB file MUST contain corrdinates of geometrically fixed Hydrogen atoms 
- preferably fixed by REDUCE v.2 or atleast compatible with the REDUCE format 
  (http://kinemage.biochem.duke.edu/downloads/software/reduce/)


#### Preparatory Step: 
The full length (FASTA) target sequence (one letter amino acid code followed by a header starting with >) should be prepared and provided alongside the coordinate (PDB) file for the model.

You can generate the fasta sequence by using:
```sh
$ EXEC/pdb2fasta.pl <input.pdb>  >  input.pdb.fasta
```

#### Run Step: 
```sh
$ ./run_ProQDock.py <model.PDB> <full-length.fasta> --flagfile myflagfile
```
where,
- model.pdb: The input pdb (coordinate file in Brrokheaven format; http://www.ccp4.ac.uk/html/procheck_man/manappb.html) file
- full-length.fasta: The full-length fasta sequence of the target

                           
> If you intend to Run ProQDock for a list of PDB files (models) corresponding to the same target sequence,
then, SKIP repeating the profile generation step more than once, by providing a [consistent basename] for the fasta file.

Launch the following bash script which runs on a set of (example) models kept at ./EXAMPLE_CASE_STUDY/
```sh
$ ./runex.bash
```

> EXAMPLE OUTPUT: 
```sh 
$ cat EXAMPLE_CASE_STUDY/D1OYV-models.status.results 
```
> 
            Model         fnat       LRMS       iRMS     CAPRI_class    DockQ    ProQDock
         D1OYV-a102a      0.000     35.348     15.846     Incorrect    0.02118    0.05908
         D1OYV-a104c      0.000     44.703     16.626     Incorrect    0.01432    0.21024
         D1OYV-a106c      0.000     44.600     18.136     Incorrect    0.01395    0.03890
         D1OYV-a115c      0.000     31.946     11.941     Incorrect    0.02722    0.02730
          D1OYV-a61c      0.652      6.820      2.334    Acceptable    0.51761    0.51060
          D1OYV-a61d      0.630      6.574      2.450    Acceptable    0.50960    0.46046
          D1OYV-a81d      0.587      6.788      2.481    Acceptable    0.48841    0.42648
          D1OYV-a41b      0.717      3.619      1.428        Medium    0.69617    0.48751
          D1OYV-a62c      0.707      3.991      1.628        Medium    0.66168    0.55795
          D1OYV-a82d      0.707      4.347      1.593        Medium    0.65639    0.48664
> 

Example Output for a single PDB file: 

```sh
$ ./run_ProQDock.py ./TESTPDBS/1u07.pdb ./TEMP/1u07.pdb.fasta --flagfile flagfile
```

```
                                                  ....                   ....                                      ..      
                                               .n~8888888nx           .xH888888Hx.                            < .z@8"`      
     .d``            .u    .          u.     :88>'8888888888:       .H8888888888888:           u.              !@88E        
     @8Ne.   .u    .d88B :@8c   ...ue888b   :8888 "*888888888k      888*"""?""*88888X    ...ue888b        .    '888E   u    
     %8888:u@88N  ="8888f8888r  888R Y888r  '88888.         "8>    'f     d8x.   ^%88k   888R Y888r  .udR88N    888E u@8NL  
      `888I  888.   4888>'88"   888R I888>   ?88888          'X    '>    <88888X   '?8   888R I888> <888'888k   888E`"88*"  
       888I  888I   4888> '     888R I888> ?  %888!           !     `:..:`888888>    8>  888R I888> 9888 'Y"    888E .dN.   
       888I  888I   4888>       888R I888>  ".:88"            !            `"*88     X   888R I888> 9888        888E~8888   
     uW888L  888'  .d888L .+   u8888cJ888     xHH8Hx.        .X  :    .xHHhx.."      !  u8888cJ888  9888        888E '888&  
    '*88888Nu88P   ^"8888*"     "*888*P"    :888888888hx....x\8..X   X88888888hx. ..!    "*888*P"   ?8888u../   888E  9888. 
    ~ '88888F`        "Y"         'Y"      :~  `"8888888888!`'8888  !   "*888888888"       'Y"       "8888P'  '"888*" 4888" 
       888 ^                                      `""*8*""`   "*"          ^"***"`                     "P'       ""    ""   
       *8E                                                                                                                  
       '8>                                                                                                                  
        "
=================================================================================================================================
Feature   Range  Direction  Description                                            Feature_Type   Stability   Native_values (DB3)
---------------------------------------------------------------------------------------------------------------------------------
EC:       [-1,1] Positive   Electrostatic balance at the interface                 Interface      Variable    0.20 (+/-0.38)
Sc:       [-1,1] Positive   Geometric / Steric fit at the interface                Interface       Stable     0.68 (+/-0.17)
rGb:      [-1,1] Positive   Distribution of hydrophobicity with respect to burial  All_Atom        Stable     0.06 (+/-0.02)
Ld:       [0, 1] Ambiguous  Link (packing) densely at the interface                Interface       Stable     0.15 (+/-0.17)
nBSA:     [0, 1] Ambiguous  Size of the interface relative to the whole complex    Interface      Variable    0.07 (+/-0.05)
Fintres:  [0, 1] Ambiguous  Size of the interface relative to the whole complex    Interface      Variable    0.19 (+/-0.13)
CPscore:  [0, 1] Positive   inter-residue contacts preference                      Interface      Variable    0.35 (+/-0.17)
CPM:      [0, 1] Positive   Likelihood of (Sc,EC) given nBSA compared to native    Interface      Variable    0.80 (+/-0.18)
rTs:      [0, 1] Negative   Energetic Stability of the whole molecule              All_Atom        Stable     0.22 (+/-0.21)
Isc:      [0, 1] Negative   Binding energy                                         Interface      Variable    0.20 (+/-0.33)
Erep:     [0, 1] Negative   Unfavourable Energy due to atomic Clashes              All_Atom        Stable     0.41 (+/-0.15)
Etmr:     [0, 1] Negative   Energetic Stability without condiderating clashes      All_Atom        Stable     0.28 (+/-0.16)
ProQ2:    [0, 1] Positive   Overall Structural Quality                             All_Atom       Variable    0.80 (+/-0.12)
---------------------------------------------------------------------------------------------------------------------------------
ProQDock  [0, 1] Positive   The predicted DockQ score, Basu and Wallner, 2016, https://doi.org/10.1371/journal.pone.0161879
   DockQ Statistics on CAPRI data:  
    0    <  DockQ <  0.23 - Incorrect
    0.23 <= DockQ <  0.49 - Acceptable quality
    0.49 <= DockQ <  0.80 - Medium quality
            DockQ >= 0.80 - High quality
=================================================================================================================================
EC=0.259
Sc=0.495
rGb=0.028
Ld=0.102
nBSA=0.054
Fintres=0.193
CPscore=0.161
CPM=0.422
Isc=0.923
rTs=0.898
Eatr=0.085
Erep=0.549
Etmr=0.873
ProQ2=0.794
==========================
ProQDock=0.266

``` 


### Reference

      Finding correct protein–protein docking models using ProQDock
      Sankar Basu and Björn Wallner* 
      Bioinformatics (2016) 32 (12): i262-i270. 
      doi: 10.1093/bioinformatics/btw257

The article is avialable online here: http://bioinformatics.oxfordjournals.org/content/32/12/i262.abstract









