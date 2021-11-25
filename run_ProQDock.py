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
flags.DEFINE_string('sc_path',None,'path to sc (Shape Complementarity') #=/software/apps/ccp4/ccp4-6.5.0/ccp4-6.5/bin
flags.DEFINE_string('delphi_path',None,'path to delphi') #=/proj/wallner/users/x_bjowa/local/DelPhi_Linux_SP_F95/
flags.DEFINE_string('rosetta',None,'path to rosetta') #=/proj/wallner/apps/rosetta/Rosetta/main/
#                    source/bin
#rosetta_db=/proj/wallner/apps/rosetta/Rosetta/main/database
#ESpath=$path/EDTSurf
flags.DEFINE_string('svm_path',None,'path to svmlight') #=/proj/wallner/users/x_bjowa/local/svm_light
flags.DEFINE_string('proqpath',None,'path to ProQ2') #/proj/wallner/users/x_bjowa/github/ProQ_scripts/bin/
flags.DEFINE_bool('diel',False,'Turn on multi dielectric Delphi')
flags.DEFINE_bool('gauss',False,'Turn on Multidielectric Gaussian (Assuming --diel is on)')

#                    proqscorepath=$rosetta_path

FLAGS=flags.FLAGS
#print(dir(flags))

#implementation of EXEC/reducemap.pl
def fix_hydrogens(pdb):
    fixed_pdb=[]
    with open(pdb,'r') as f:
        for line in f.readlines():
            line=line.rstrip()
            if line.startswith('ATOM'):
                atom=line[12:16]
               # print(line)
               # print(atom)
               # print(len(atom))
                if atom[0] == 'H' or atom[1] == 'H':
                    if atom[1] == 'H' and atom[0] != 'H':
                        #correct format
                        #fixed_pdb.append(line)
                        pass
                    elif atom[0] == 'H':
                        reorder_atom = atom[3]+atom[0:3] #substr($atom,3,1).substr($atom,0,3);
                        #print(reorder_atom)
                        #print('A:',line)
                        line = line[0:12]+reorder_atom+line[16:]
                        #print('B:',line)
#                        fixed_pdb.append(line)
                    else:
                        print('Hello',line)
                    #    pass
                
            fixed_pdb.append(line)
            
                

    
    return "\n".join(fixed_pdb)

def calc_EC(pdb,delphi_path=None,ESpath=None,diel=False,gauss=False):
    cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    amber_crg=os.path.join(PATH,'LIBR','amber.crg')
    amber_dummy=os.path.join(PATH,'LIBR','amber_dummy.siz')
    with tempfile.TemporaryDirectory() as tmpdir:
        pdb_str=fix_hydrogens(pdb)
        with open('leffe','w') as f:
            f.write(pdb_str)
        os.chdir(tmpdir)
        with open('input.pdb','w') as f:
            f.write(pdb_str)
        os.symlink(amber_crg,os.path.basename(amber_crg))
        os.symlink(amber_dummy,os.path.basename(amber_dummy))
        os.system('pwd;ls -lrt')

        print(pdb)
    os.chdir(cwd)
        #print(tmpdir)
    
    

def main(argv):
    if len(argv) != 3:
        print('./run_ProQDock.py <pdb> <fasta> <options>')
        print('You need to supply a pdb and fasta file')

    pdb=argv[1]
    fasta=argv[2]
    rosetta_path=os.path.join(FLAGS.rosetta,'source','bin')
    proqscorepath=rosetta_path
    ESpath=os.path.join(os.path.abspath(os.path.dirname(__file__)),'EDTSurf')
    rosetta_db=os.path.join(FLAGS.rosetta,'database')

    calc_EC(pdb,delphi_path=FLAGS.delphi_path,ESpath=ESpath,diel=FLAGS.diel,gauss=FLAGS.gauss)
    #print(dir(tempfile))
    


    
  # print(FLAGS)
    
 #  print(argv)
        


              
   


if __name__ == '__main__':
   # flags.mark_flags_as_required(['pdb','fasta'])
    app.run(main)
