#!/usr/bin/env python
import glob
import os
import sys
import re
import tempfile
import subprocess
#import numpy as np
from operator import itemgetter

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

#implementation of EXEC/metRrename.csh
def fix_metals(pdb_str):

    metals={'NA    NA':' NA1 SOD',
	    'MG    MG':' MG2 MAG',
	    'AL   ALF':' AL3 ALM',
	    ' K     K':' K_1 POT',
	    'CA    CA':' CA2 CAL',
	    'MN    MN':' MN2 MNG',
	    'MN   MN3':' MN3 MNG',
	    'FE   FE2':' FE2 IRN',
	    'FE    FE':' FE3 IRN',
	    'CO    CO':' CO2 COB',
	    'CO   3CO':' CO3 COB',
	    'NI    NI':' NI2 NIC',
	    'NI   3NI':' NI3 NIC',
	    'CU   CU1':' CU1 COP',
	    'CU    CU':' CU2 COP',
	    'ZN    ZN':' ZN2 ZNC',
	    'AG    AG':' AG1 SLV',
	    'CD    CD':' CD2 CDM',
	    'PT1  TPT':' PT2 PLT',
	    'AU    AU':' AU1 GLD',
	    'AU   AU3':' AU3 GLD',
            'HG    HG':' HG2 MRC'}
    hydrogen={' HT1':' H  ',
              ' HT2':' H  ',
              ' HT3':' H  '}
              

    for pat in metals:
        pdb_str=pdb_str.replace(pat,metals[pat])

    for pat in hydrogen:
        pdb_str=pdb_str.replace(pat,hydrogen[pat])

    #return pdb_str.translate(metals)
    return pdb_str

#implementation of EXEC/reducemap.pl
def read_pdb(pdb):
    fixed_pdb=[]
    chains={}
    chain='extra'
    with open(pdb,'r') as f:
        for line in f.readlines():
            #line=line.rstrip()
            if line.startswith('ATOM'):
                atom=line[12:16]
                chain=line[21]
                if chain not in chains:
                    chains[chain]=[]
                #print(chain)
                #print(line)
                #print(atom)
               # print(len(atom))
                #implementation of EXEC/reducemap.pl
                if atom[0] == 'H' or atom[1] == 'H':
                    if atom[1] == 'H' and atom[0] != 'H':
                        #correct format
                        #fixed_pdb.append(line)
                        pass
                    elif atom[0] == 'H':
                        reorder_atom = atom[3]+atom[0:3] #substr($atom,3,1).substr($atom,0,3);
                        #print(reorder_atom)
                        #print('A:',lx
                        line = line[0:12]+reorder_atom+line[16:]
                        #print('B:',line)
#                        fixed_pdb.append(line)
                    else:
                        #print('Hello',line)
                        pass
            chains[chain].append(line)
            fixed_pdb.append(line)
            
    
    #s=fix_metals('HG    HG\nFE   FE2')
    #print(s)
    #return pose
    for chain in chains:
        chains[chain]="".join(chains[chain])
    
    return (fix_metals("".join(fixed_pdb)),chains)

def run_naccess(pdb_str):
    print(__file__)
    cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    naccess=os.path.join(PATH,'EXEC','naccess.bash')
    with tempfile.TemporaryDirectory() as tmpdir:
        os.chdir(tmpdir)
        with open('input.pdb','w') as f:
            f.write(pdb_str)
        cmd=f'{naccess} input.pdb'
        subprocess.check_output(cmd, shell=True)
        os.system('ls -lrt')
        with open('input.asa','r') as f:
            asa=f.read()

    os.chdir(cwd)
    return asa
#ATOM      1  N   ALA A   1       8.135 -22.074  30.824  37.807  1.65

def read_asa(asa_str):
    asa={}

    for line in asa_str.split('\n')[:-1]: #last element will be empty
        line=line.rstrip()
        if line.startswith('ATOM'):
            key=line[13:27] #.lstrip()
            saa=line[54:62]
            asa[key]=float(saa)
            #print(line)
            #print(key,' --> ', asa[key])
    return asa
#ATOM    3715
#EXEC/delasa.f
def get_interface(AB,A,B):
    asa_AB=read_asa(AB)
    asa_A=read_asa(A)
    asa_B=read_asa(B)
    interface_A=[]
    interface_B=[]
    dasa_A=0
    dasa_B=0
    
    for atom in asa_A:
        if atom in asa_AB: #should always be true...
            dasa=asa_A[atom]-asa_AB[atom] #Area will always be more exposed (>) in chain than in complex, thus diff should never be negative.
            if dasa > 0:
                dasa_A+=dasa
                interface_A.append(atom)
    for atom in asa_B:
        if atom in asa_AB: #should always be true...
            dasa=asa_B[atom]-asa_AB[atom] #Area will always be more exposed (>) in chain than in complex, thus diff should never be negative.
            if dasa > 0:
                dasa_B+=dasa
                interface_B.append(atom)
                
            
                #print(dsaa)
        
    int_tot=len(interface_A)+len(interface_B)
    
    #print(int_tot,dasa_A,dasa_B,(dasa_A+dasa_B)/2)
    return(set(interface_A),set(interface_B),dasa_A,dasa_B,(dasa_A+dasa_B)/2)
def by_third(a):
    cols=a.split()
    return (int(cols[2]),cols[0])

def convert_surf_to_pdb(surf_str):
    pdb_lines=[]
    #print(surf_str)
    #sys.exit()
    #sort surf_str
    surf_lst=surf_str.split('\n')[:-1]
#    s=sorted(surf_lst,key=by_third)
 #   i=1
    for line in sorted(surf_lst,key=by_third):
        #splitting line shoulw work
        #print(line)
        #print(i)
        
        (atom,res,resnum,chain,x,y,z,r,g,b)=line.strip().split()
        i=''
        pdb_lines.append(f'ATOM  {i:>5s}  {atom:3} {res:3} {chain:1}{resnum:>4}    {x:>8}{y:>8}{z:>8}\n')
#        i+=1
    #sys.exit()

    return pdb_lines

def run_EDTSurf(pdb_str):
    print(__file__)
    cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    EDTSurf=os.path.join(PATH,'EDTSurf','EDTSurf')
    with tempfile.TemporaryDirectory() as tmpdir:
        os.chdir(tmpdir)
        with open('input.pdb','w') as f:
            f.write(pdb_str)
        cmd=f'{EDTSurf} -i input.pdb -s 3 -f 1 -o out > EDTSurf.log'
        os.system(cmd)
#        subprocess.check_output(cmd, shell=True)
        os.system('ls -lrt')
        #os.system('cp out.ply.surf /home/x_bjowa/proj/local/ProQDock/tmp.ply.surf')
        with open('out.ply.surf','r') as f:
            surf=convert_surf_to_pdb(f.read())

    os.chdir(cwd)
    return surf
    


    
def calc_EC(pdb_str,pdb_chains,tmpdir,delphi_path=None,ESpath=None,diel=False,gauss=False):
    #cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    amber_crg=os.path.join(PATH,'LIBR','amber.crg')
    amber_dummy=os.path.join(PATH,'LIBR','amber_dummy.siz')
#    os.chdir(tmpdir)
    asa={}
    grid={}
    asa1=run_naccess(pdb_str)
    chains=sorted(pdb_chains.keys()) #as of python 3 dicts keys are not random put in the order they were created, so this is not needed...
    for chain in chains:
        asa[chain]=run_naccess(pdb_chains[chain])
        grid[chain]=run_EDTSurf(pdb_chains[chain])
    (interface_A,interface_B,dA,dB,interface_area)=get_interface(asa1,asa[chains[0]],asa[chains[1]])

    gridA=[p for p in grid[chains[0]] if p[13:27] in interface_A] #intsurf1.pdb
    gridB=[p for p in grid[chains[1]] if p[13:27] in interface_B] #intsurf2.pdb
    
#    gridA=[p[13:27] for p in grid[chains[0]]]
    #print(grid[chains[0]])
    with open('tmp.pdb','w') as f:
        f.write("".join(gridA))
    
    
    
   # print(gridA)
   # print(interface_A)
    os.chdir(tmpdir)
    os.symlink(amber_crg,os.path.basename(amber_crg))
    os.symlink(amber_dummy,os.path.basename(amber_dummy))
    os.system('pwd;ls -lrt')

    
    #os.chdir(cwd)
        #print(tmpdir)
    
    

def main(argv):
    if len(argv) != 3:
        print('./run_ProQDock.py <pdb> <fasta> <options>')
        print('You need to supply a pdb and fasta file')

    input_pdb=argv[1]
    fasta=argv[2]
    rosetta_path=os.path.join(FLAGS.rosetta,'source','bin')
    proqscorepath=rosetta_path
    ESpath=os.path.join(os.path.abspath(os.path.dirname(__file__)),'EDTSurf')
    rosetta_db=os.path.join(FLAGS.rosetta,'database')

    (pdb_str,pdb_chains)=read_pdb(input_pdb)

    if len(pdb_chains.keys()) != 2:
        print("ProQDock only works for two chains. You group chains to hack it evalute different interfaces\n")
        print("Currently you have to do that outside, support for that might be added in the future\n")
        sys.exit(1)

    logging.info(f"CHAINS: {sorted(pdb_chains.keys())}")
   
    
    #pdb=input_pdb
    with tempfile.TemporaryDirectory() as tmpdir:
        calc_EC(pdb_str,pdb_chains,tmpdir,delphi_path=FLAGS.delphi_path,ESpath=ESpath,diel=FLAGS.diel,gauss=FLAGS.gauss)
    #print(dir(tempfile))
    


    
  # print(FLAGS)
    
 #  print(argv)
        


              
   


if __name__ == '__main__':
   # flags.mark_flags_as_required(['pdb','fasta'])
    app.run(main)
