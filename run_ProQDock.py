#!/usr/bin/env python
import glob
import os
import sys
import re
import tempfile
#import numpy as np

from absl import app
from absl import flags
from absl import logging


#flags.DEFINE_string('pdb',None,'Path to pdbfile')
#flags.DEFINE_string('fasta',None,'Path to sequence for full length sequence (will reuse these if multiple models are run). First time the sequene specific features will be calculated and stored at the location of the fasta file')
flags.DEFINE_string('outfile',None,'outfile otherwise stdout')
flags.DEFINE_string('sc_path'=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin
flags.DEFINE_string('delphi_path'None,'path to delphi') #=/proj/wallner/users/x_bjowa/local/DelPhi_Linux_SP_F95/
flags.DEFINE_string('rosetta_path',None,'path to rosetta') #=/proj/wallner/apps/rosetta/Rosetta/main/
#                    source/bin
#rosetta_db=/proj/wallner/apps/rosetta/Rosetta/main/database
#ESpath=$path/EDTSurf
flags.DEFINE_string('svm_path',None,'path to svmlight') #=/proj/wallner/users/x_bjowa/local/svm_light
flags.DEFINE_string('proqpath',None,'path to ProQ2') #/proj/wallner/users/x_bjowa/github/ProQ_scripts/bin/

#                    proqscorepath=$rosetta_path

FLAGS=flags.FLAGS
print(dir(flags))
def main(argv):
    if len(argv) != 3:
        print('./run_ProQDock.py <pdb> <fasta> <options>')
        print('You need to supply a pdb and fasta file')
  # print(FLAGS)
    
 #  print(argv)
        


              
   


if __name__ == '__main__':
   # flags.mark_flags_as_required(['pdb','fasta'])
    app.run(main)
