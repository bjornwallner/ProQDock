#!/usr/bin/env python
import glob
import os
import sys
import re
import tempfile
import subprocess
import numpy as np
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
flags.DEFINE_bool('AF',False,'[Experimental use at own risk]. For AlphaFold models. This will use the average B-factor (plDDT) in the model instead of ProQ2. (No need to give fasta sequence)')

#                    proqscorepath=$rosetta_path

FLAGS=flags.FLAGS
#print(dir(flags))
three_to_one = {'ARG': 'R', 'HIS': 'H', 'LYS': 'K', 'ASP': 'D', 'GLU': 'E', 
                'SER': 'S', 'THR': 'T', 'ASN': 'N', 'GLN': 'Q', 'CYS': 'C', 
                'GLY': 'G', 'PRO': 'P', 'ALA': 'A', 'VAL': 'V', 'ILE': 'I', 
                'LEU': 'L', 'MET': 'M', 'PHE': 'F', 'TYR': 'Y', 'TRP': 'W'}

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

# Renames HIS to HID, HIE, or HIP dependent on the hydrogens present from EXEC/his2hidep.pl
def fix_his(pdb_str):

    res_map={}
    old_resnum='undef'
    old_res='UNK'
    residues=[]
    pdb=[]
    d=0
    e=0
    for line in pdb_str.split('\n')[:-1]:
        if line.startswith('ATOM'):
            atom=line[12:16].strip()
            res=line[17:20]
            resnum=line[22:26]
            if  resnum != old_resnum and old_resnum != 'undef': # new res
                if old_res =='HIS':
                    his='HID'
                    if d == 1 and e == 1:
                        his='HIP'
                    if d == 0 and e == 1:
                        his='HIE'                        
                    residues=[line.replace('HIS',his) for line in residues]
                pdb+=residues
                residues=[]
                d=0
                e=0

            if res=='HIS':
                if atom == 'HD1':
                    d=1
                elif atom == 'HE2':
                    e=1
            old_resnum=resnum
            old_res=res
            residues.append(line+'\n')
                      
    if len(residues) > 0:
        if old_res == 'HIS':
            his='HID'
            if d == 1 and e == 1:
                his='HIP'
            if d == 0 and e == 1:
                his='HIE'                        
            residues=[line.replace('HIS',his) for line in residues]
        pdb+=residues

    return("".join(pdb))
            
            
        
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
                line=line[0:12] + ' ' + line[13:] #remove numbers before hydrogens, Delphi does not seem to like them
                chains[chain].append(line)
                fixed_pdb.append(line)
            
    
    #s=fix_metals('HG    HG\nFE   FE2')
    #print(s)
    #return pose
    for chain in chains:
        chains[chain]=fix_metals("".join(chains[chain]))
    pdb_data={}
    pdb_data['pdb_str']=fix_metals("".join(fixed_pdb))
    pdb_data['pdb_chains']=chains

    if len(pdb_data['pdb_chains'].keys()) != 2:
        print("ProQDock only works for two chains. You group chains to hack it evalute different interfaces\n")
        print("Currently you have to do that outside, support for that might be added in the future\n")
        sys.exit(1)
    logging.info(f"Found these chains: {sorted(pdb_data['pdb_chains'].keys())}")

    (pdb_data['asa_pdb_str'],pdb_data['rsa_pdb_str'])=run_naccess(pdb_data['pdb_str'])
    chains=sorted(pdb_data['pdb_chains'].keys()) #as of python 3 dicts keys are not random put in the order they were created, so this is not needed...
    pdb_data['asa_chain']={}
    for chain in chains:
        logging.info(f'NACCESS for chain {chain}')
        pdb_data['asa_chain'][chain],_=run_naccess(pdb_data['pdb_chains'][chain])


    (interface_A,interface_B,dA,dB,interface_area,total_area)=get_interface(pdb_data['asa_pdb_str'],pdb_data['asa_chain'][chains[0]],pdb_data['asa_chain'][chains[1]])
    pdb_data['interface_A']=interface_A
    pdb_data['interface_B']=interface_B
    pdb_data['interface_A_area']=dA
    pdb_data['interface_B_area']=dB
    pdb_data['interface_area']=interface_area #mean of the two above
    pdb_data['total_area']=total_area #total exposed surface area of complex
    
    #return (fix_metals("".join(fixed_pdb)),chains)
    return(pdb_data)

def run_naccess(pdb_str):
   # print(__file__)
    cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    naccess=os.path.join(PATH,'EXEC','naccess.bash')
    with tempfile.TemporaryDirectory() as tmpdir:
        os.chdir(tmpdir)
        with open('input.pdb','w') as f:
            f.write(pdb_str)
        cmd=f'{naccess} input.pdb'
        subprocess.check_output(cmd, shell=True)
#        os.system('ls -lrt')
        with open('input.asa','r') as f:
            asa=f.read()
        with open('input.rsa','r') as f:
            rsa=f.read()
            
    os.chdir(cwd)
    return asa,rsa
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

def read_rsa(rsa_str):
    rsa={}

    lines=rsa_str.split('\n')[:-1]
    lines=[line for line in lines if line.startswith('RES')]
    #print(len(lines))
    
    exposure = [(three_to_one[line.split()[1]],float(line[16:22].rstrip())) for line in lines]
    return(exposure)
    


def get_interface(AB,A,B):
    asa_AB=read_asa(AB)
    asa_A=read_asa(A)
    asa_B=read_asa(B)
    interface_A=[]
    interface_B=[]
    dasa_A=0
    dasa_B=0
    total_area=0

    for atom in asa_AB:
        total_area+=asa_AB[atom]
    
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
    return(set(interface_A),set(interface_B),dasa_A,dasa_B,(dasa_A+dasa_B)/2,total_area)

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
#    print(__file__)
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
        #os.system('ls -lrt')
        #os.system('cp out.ply.surf /home/x_bjowa/proj/local/ProQDock/tmp.ply.surf')
        with open('out.ply.surf','r') as f:
            surf=convert_surf_to_pdb(f.read())
    os.chdir(cwd)
    return surf


def rename_nc_terminals(pdb_str):
    aa3 = ['GLY','ALA','VAL','LEU','ILE','PHE','TYR','TRP','SER','THR','CYS','CYX','MET','ASP','GLU','ASN','GLN','LYS','ARG','PRO','HID','HIE','HIP'];
    aan = ['GNN','ANN','VNN','LNN','INN','FNN','YNN','WNN','SNN','TNN','CSN','CXN','MNN','DNN','ENN','NNN','QNN','KNN','RNN','PNN','HDN','HEN','HPN'];
    aac = ['GCC','ACC','VCC','LCC','ICC','FCC','YCC','WCC','SCC','TCC','CSC','CXC','MCC','DCC','ECC','NCC','QCC','KCC','RCC','PCC','HDC','HEC','HPC'];
    aan_dict=dict(zip(aa3,aan))
    aac_dict=dict(zip(aa3,aac))
    pdb_lines=pdb_str.split('\n')
    resnum_N=pdb_lines[0][22:26]
    resnum_C=pdb_lines[-2][22:26]
#    print(pdb_lines)
#    print(resnum_N,resnum_C)
    new_pdb=[]
    for line in pdb_lines[:-1]:
        res=line[17:20]
        resnum=line[22:26]
        if resnum == resnum_N:
            line=line.replace(res,aan_dict[res])
        if resnum == resnum_C:
            line=line.replace(res,aac_dict[res])
            
        new_pdb.append(line +'\n')
    return "".join(new_pdb)

def dummy_pdb(pdb_str):
    res = ['GLY','ALA','VAL','LEU','ILE','PHE','TYR','TRP','SER','THR','CYS','CYX','MET','ASP','GLU','ASN','GLN','LYS','ARG','PRO','HID','HIE','HIP','GNN','ANN','VNN','LNN','INN','FNN','YNN','WNN','SNN','TNN','CSN','CXN','MNN','DNN','ENN','NNN','QNN','KNN','RNN','PNN','HDN','HEN','HPN','GCC','ACC','VCC','LCC','ICC','FCC','YCC','WCC','SCC','TCC','CSC','CXC','MCC','DCC','ECC','NCC','QCC','KCC','RCC','PCC','HDC','HEC','HPC','SOD','MAG','ALM','POT','CAL','CRM','MNG','IRN','COB','NIC','COP','ZNC','SLV','CDM','PLT','GLD','MRC']
    dumm = ['GDD','ADD','VDD','LDD','IDD','FDD','YDD','WDD','SDD','TDD','CSD','CXD','MDD','DDD','EDD','NDD','QDD','KDD','RDD','PDD','HDD','HED','HPD','GDD','ADD','VDD','LDD','IDD','FDD','YDD','WDD','SDD','TDD','CSD','CXD','MDD','DDD','EDD','NDD','QDD','KDD','RDD','PDD','HDD','HED','HPD','GDD','ADD','VDD','LDD','IDD','FDD','YDD','WDD','SDD','TDD','CSD','CXD','MDD','DDD','EDD','NDD','QDD','KDD','RDD','PDD','HDD','HED','HPD','DSO','DMG','DAL','DPT','DCA','DCR','DMN','DIR','DCO','DNI','DCU','DZN','DSL','DCD','DPL','DGL','DMR']
    dummy=dict(zip(res,dumm))
    pdb_lines=pdb_str.split('\n')
    new_pdb=[]
    for line in pdb_lines[:-1]:
        res=line[17:20]
        line=line.replace(res,dummy[res])
        new_pdb.append(line +'\n')
    return "".join(new_pdb)


def convert_atom_to_res(interface):
    residues=[]
    for atom_record in interface:
        (atom,res,chain,resnum)=atom_record.split()
        residues.append(f'{resnum:>3} {res} {chain}')
    return(set(residues))
    


def remove_hydrogen(pdb_str):
    pdb_lines=[]
    for line in pdb_str.split('\n')[:-1]:
        atom=line[12:16]
        if 'H' not in atom:
            pdb_lines.append(line+'\n')
    return "".join(pdb_lines)
        
    
def calc_Sc(pdb_data,tmpdir,sc_path):
    logging.info('Starting Sc calculation')
    run_sc=os.path.join(tmpdir,'run_sc')
    pdb=os.path.join(tmpdir,'input.pdb')
    with open(pdb,'w') as f:
        f.write(remove_hydrogen(pdb_data['pdb_str']))
    
    chains=sorted(pdb_data['pdb_chains'].keys())
    ccp4base=os.path.dirname(os.path.dirname(sc_path))
    ccplib=os.path.join(ccp4base,'lib','data')
    ccpinclude=os.path.join(ccp4base,'include')
    with open(run_sc,'w') as f:
        f.write(f'#!/bin/bash\n')
        f.write(f'export CCP4_SCR={tmpdir}\n')
        f.write(f'export CLIBD={ccplib}\n')
        f.write(f'export CINCL={ccpinclude}\n')
        
        f.write(f'{sc_path} XYZIN {pdb} <<eof\n')
        f.write(f'MOLECULE 1\n')
        f.write(f'CHAIN {chains[0]}\n')
        f.write(f'MOLECULE 2\n')
        f.write(f'CHAIN {chains[1]}\n')
        f.write(f'END\n')
        f.write(f'eof\n')

    Sc=subprocess.check_output(f"source {run_sc}|grep 'Sc ='", shell=True,stderr=subprocess.STDOUT).decode('UTF-8').strip()
    Sc=float(Sc.split()[-1])
    return(Sc)

def calc_CPscore(pdb_data,tmpdir):
    logging.info('Calculating CPscore')
    cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    contpref20CB=os.path.join(PATH,'EXEC','contpref20CB.exe')
    contpref_mat=os.path.join(PATH,'LIBR','contpref.mat')
    generate_svm_param=os.path.join(PATH,'EXEC','contpref20svm.pl')
    svm_model_paths=os.path.join(PATH,'SVMmodels.CPscore','*.model')
    svm_classify=os.path.join(FLAGS.svm_path,'svm_classify')
    runSVM=os.path.join(PATH,'EXEC','cal.svmCPS')
    os.chdir(tmpdir)
    if not os.path.exists('input.pdb'):
        with open('input.pdb','w') as f:
            f.write(pdb_data['pdb_str'])


    with open('interface-A.res','w') as f:
        f.write("\n".join(sorted(convert_atom_to_res(pdb_data['interface_A']))))
        f.write("\n")
    with open('interface-B.res','w') as f:
        f.write("\n".join(sorted(convert_atom_to_res(pdb_data['interface_B']))))
        f.write("\n")
        
        
    cmd=f'touch fort.dummy;{contpref20CB} input.pdb interface-A.res interface-B.res {contpref_mat};cp fort.37 input.contpref20'
    cmd2=f'{generate_svm_param} input.contpref20'
    #logging.info(f'CMD: {cmd}')
    subprocess.check_output(cmd, shell=True).decode('UTF-8').strip()
    #logging.info(f'CMD: {cmd2}')
    subprocess.check_output(cmd2, shell=True).decode('UTF-8').strip()
    svm_input_file='input-C0.svm1'
    preds=[]
    svm_outputs=[]
    cmds=[]
    for i,svm_model in enumerate(glob.glob(svm_model_paths)):
        #print(svm_model)
        svm_output=f'{svm_input_file}.{i}'
        cmd=f'{svm_classify} {svm_input_file} {svm_model} {svm_output} > /dev/null &'
        cmds.append(cmd)
        svm_outputs.append(svm_output)

    cmds.append('wait')
    os.system("".join(cmds))
    for svm_output in svm_outputs:
        with open(svm_output) as f:
            pred=f.read().rstrip().split()[0]
            #pred=subprocess.check_output(f'cat {svm_output}', shell=True,stderr=subprocess.STDOUT).decode('UTF-8').rstrip().split()[0]
            preds.append(float(pred))
        
    #cmd3=f'{runSVM} input-C0.svm1 {FLAGS.svm_path} > /dev/null ;cat input-C0.CPS'
    #subprocess.check_output(cmd2, shell=True).decode('UTF-8').strip()
    #logging.info(f'CMD: {cmd3}')
    #   print(gsz2)
    #CPscore=subprocess.check_output(cmd3, shell=True).decode('UTF-8').strip()
    CPscore=np.mean(preds)
#    print(gsz3)
#    os.system('ls -lrt')
    os.chdir(cwd)
    return CPscore


def dist(a,b):
    return np.sqrt(np.sum((a-b)**2))

def _maxdist(pdb_str):
    pdb_lines=pdb_str.split('\n')
    new_pdb=[]
    coords=[]
    mdist=0
    for line in pdb_lines[:-1]:
        if line.startswith('ATOM'):
            x=float(line[30:38])
            y=float(line[38:46])
            z=float(line[47:54])
            #print(line)
            #print(x,y,z)
            c=[x,y,z]
            coords.append([x,y,z])
    a=np.array(coords)
    m = np.sqrt(np.sum((a[:,np.newaxis,:] - a[np.newaxis,:,:])**2, axis=2))
    mdist=m.max()
    return mdist


def read_delphi_potential(pot):
    get_line=False
    pots=[]
    with open(pot,'r') as f:
        for line in f.readlines():
            if 'total energy' in line:
                get_line=False

            if get_line:    
                pot=line.split()[8]
                pots.append(float(pot))    
            if line.startswith('ATOM DESCRIPTOR'):
                get_line=True
    return(pots)

def EC_corr(pot1,pot2):
    pots1=read_delphi_potential(pot1)
    pots2=read_delphi_potential(pot2)
    c=np.corrcoef(pots1,pots2)[1,0]
    return(-c)
    
           

def calc_EC(pdb_data,tmpdir,delphi_path=None,diel=False,gauss_delphi=False):
    logging.info('Starting EC calculation')
    cwd=os.getcwd()
    PATH=os.path.abspath(os.path.dirname(__file__))
    amber_crg=os.path.join(PATH,'LIBR','amber.crg')
    amber_dummy=os.path.join(PATH,'LIBR','amber_dummy.siz')
#    os.chdir(tmpdir)
 #   asa={}
    grid={}
#    logging.info(f'NACCESS for input pdb')
#    asa1,rsa1=run_naccess(pdb_str)
    chains=sorted(pdb_data['pdb_chains'].keys()) #as of python 3 dicts keys are not random put in the order they were created, so this is not needed...
    for chain in chains:
#        logging.info(f'NACCESS for chain {chain}')
#        asa[chain],_=run_naccess(pdb_chains[chain])
        logging.info(f'Calculating grid points for chain {chain}')
        grid[chain]=run_EDTSurf(pdb_data['pdb_chains'][chain])
    
    gridA=[p for p in grid[chains[0]] if p[13:27] in pdb_data['interface_A']] #intsurf1.pdb
    gridB=[p for p in grid[chains[1]] if p[13:27] in pdb_data['interface_B']] #intsurf2.pdb

    os.chdir(tmpdir)
    with open('gridA.pdb','w') as f:
        f.write("".join(gridA))
    with open('gridB.pdb','w') as f:
        f.write("".join(gridB))
    
#    gridA=[p[13:27] for p in grid[chains[0]]]
    #print(grid[chains[0]])
    chain1=fix_his(pdb_data['pdb_chains'][chains[0]])
    chain2=fix_his(pdb_data['pdb_chains'][chains[1]])
    pdb1=rename_nc_terminals(chain1)
    pdb1_dummy=dummy_pdb(chain1)
    pdb2=rename_nc_terminals(chain2)
    pdb2_dummy=dummy_pdb(chain2)
    with open('A_maskedB.pdb','w') as f:
        #f.write(pdb_chains['A'])
        f.write(pdb1)
        f.write(pdb2_dummy)
    with open('maskedA_B.pdb','w') as f:
        f.write(pdb1_dummy)
        f.write(pdb2)
    with open('input.pdb','w') as f:
        f.write(pdb_data['pdb_str'])

    
    delphi_script=os.path.join(PATH,'EXEC','generateprm26.pl')
    gsz=_maxdist(pdb_data['pdb_str'])+25
    #print(gsz)
    gsz=int(float(gsz))
    gauss=0
    if gauss_delphi:
        gauss=1


    os.symlink(amber_crg,os.path.basename(amber_crg))
    os.symlink(amber_dummy,os.path.basename(amber_dummy))
    logging.info(f'Running Delphi for gridA using {delphi_path}')
    cmd=f'{delphi_script} {tmpdir} A_maskedB.pdb gridA.pdb outmod1.pdb outsurf11.pot {gsz} {gauss}'
    cmd2=f'{delphi_path} script.prm > log11;rm -f ARCDAT'
    os.system(cmd)
    os.system(cmd2)
    cmd=f'{delphi_script} {tmpdir} maskedA_B.pdb gridA.pdb outmod2.pdb outsurf21.pot {gsz} {gauss}'
    cmd2=f'{delphi_path} script.prm > log21;rm -f ARCDAT'
    os.system(cmd)
    os.system(cmd2)
    corr1=EC_corr('outsurf11.pot','outsurf21.pot')

    
    logging.info(f'Running Delphi for gridB using {delphi_path}')
    cmd=f'{delphi_script} {tmpdir} A_maskedB.pdb gridB.pdb outmod1.pdb outsurf12.pot {gsz} {gauss}'
    cmd2=f'{delphi_path} script.prm > log12;rm -f ARCDAT'
    os.system(cmd)
    os.system(cmd2)
    cmd=f'{delphi_script} {tmpdir} maskedA_B.pdb gridB.pdb outmod2.pdb outsurf22.pot {gsz} {gauss}'
    cmd2=f'{delphi_path} script.prm > log22;rm -f ARCDAT'
    os.system(cmd)
    os.system(cmd2)
    corr2=EC_corr('outsurf12.pot','outsurf22.pot')

    EC=(float(corr1)+float(corr2))/2

    
    #os.system('cp * /home/x_bjowa/proj/local/ProQDock/foo100/')
    #sys.exit()
    os.chdir(cwd)
    return EC

    
def calc_rGb(pdb_data):
    Log10PropResBur = {'A': [0.1576, -0.0097, -0.0872, -0.1158], 
                   'C': [0.2842, 0.1824, -0.0851, -0.6021], 
                   'D': [-0.2883, -0.1158, 0.0233, 0.1688], 
                   'E': [-0.5952, -0.2111, -0.0334, 0.2271], 
                   'F': [0.2068, 0.1611, -0.0329, -0.3458], 
                   'H': [-0.1051, 0.0711, 0.1069, -0.0057], 
                   'I': [0.2707, 0.0792, -0.0985, -0.3969], 
                   'K': [-0.9830, -0.3958, -0.0706, 0.2767], 
                   'L': [0.2271, 0.0973, -0.0223, -0.3478], 
                   'M': [0.1726, 0.0330, -0.0209, -0.1938], 
                   'N': [-0.3054, -0.0825, 0.0378, 0.1392], 
                   'P': [-0.2027, -0.0655, 0.0245, 0.1096], 
                   'Q': [-0.4237, -0.1385, 0.0422, 0.1735], 
                   'R': [-0.6383, -0.0899, 0.1129, 0.1741], 
                   'S': [-0.0762, -0.0119, 0.0179, 0.0441], 
                   'T': [-0.0501, -0.0110, 0.0492, 0.0158], 
                   'V': [0.2480, 0.0603, -0.0783, -0.3279], 
                   'W': [0.1014, 0.2467, 0.1075, -0.3391], 
                   'Y': [0.0090, 0.2274, 0.1565, -0.2284]} #Use index1 if rASA <= 0.05, index2 if 0.05 < rASA <= 0.15, index3 if 0.15 < rASA <= 0.30, and index4 if rASA >= 0.30
    ASA_normalisation = {'A': 123.4115, 'C': 147.4431, 'D': 163.7097, 'E': 195.2893, 'F': 203.8527, 
                     'G': 93.5631, 'H': 198.1719, 'I': 179.6625, 'K': 223.2443, 'L': 193.7956, 
                     'M': 217.3316, 'N': 161.8336, 'P': 159.6743, 'Q': 195.3820, 'R': 256.8007, 
                     'S': 135.2496, 'T': 155.6512, 'V': 163.5877, 'W': 252.3393, 'Y': 234.5152}

    residue_exposure=read_rsa(pdb_data['rsa_pdb_str'])
    n_residues = 0 #len(residue_exposure)
    rGb = 0
    
    for residue,exposure, in residue_exposure:
        if residue == 'G':
            continue
        n_residues+=1
        relative_exposure = round(exposure/ASA_normalisation[residue],2)
       # print(residue,exposure,relative_exposure)
        #relative_exposure = exposure[index]/ASA_renormalisation[residue]
        prop=0
        if relative_exposure > 0.30:
            prop= Log10PropResBur[residue][3]
            tag=4
        elif relative_exposure > 0.15:
            prop= Log10PropResBur[residue][2]
            tag=3
        elif relative_exposure > 0.05:
            prop= Log10PropResBur[residue][1]
            tag=2
        else:
            prop= Log10PropResBur[residue][0]
            tag=1
       # print(f'RES: {residue:3} {relative_exposure:.2f} {tag} {10**prop:.3f}')
        rGb+=prop
    if n_residues == 0:
        return 0
    rGb = rGb / n_residues
    return rGb





def calc_Ld(pdb_str,tmpdir):
    PATH=os.path.abspath(os.path.dirname(__file__))
    Ld=os.path.join(PATH,'MAINEXEC','ldN.exe')
    pdb=os.path.join(tmpdir,'input.pdb')
#    if not os.path.exists(pdb):
    with open(pdb,'w') as f:
        f.write(pdb_str)         
    
    
    os.system(f'cd {tmpdir};grep ^ATOM {pdb} > input.Ld.pdb;{Ld} input.Ld.pdb > /dev/null')
    Ld=subprocess.check_output(f"cat {tmpdir}/fort.130", shell=True,stderr=subprocess.STDOUT).decode('UTF-8').strip()
    return(float(Ld))

def calc_CPM(Sc,EC,nBSA):
    logging.info('Starting CPM calculation')
    PATH=os.path.abspath(os.path.dirname(__file__))
    CPMpl=os.path.join(PATH,'MAINEXEC','CPMgScEC.pl')
    cmd=f"{CPMpl} {PATH} {Sc} {EC} {nBSA}"
    CPM=subprocess.check_output(f'{cmd}', shell=True,stderr=subprocess.STDOUT).decode('UTF-8').strip()
    
    return float(CPM)


def calc_rosetta_terms(pdb_str,tmpdir,rosetta_path,rosetta_db):
    logging.info('Starting Rosetta calculation')
    PATH=os.path.abspath(os.path.dirname(__file__))
    rosettaE=os.path.join(PATH,'MAINEXEC','run.rosettaE')
    relax_script=os.path.join(PATH,'MAINEXEC','repack.script')
    
    pdb=os.path.join(tmpdir,'input.pdb')
#    if not os.path.exists(pdb):
    with open(pdb,'w') as f:
        f.write(pdb_str)
    cmd=f'cd {tmpdir};{rosettaE} input.pdb {rosetta_path} {rosetta_db}'
    score=subprocess.check_output(f'{cmd}|tail -n 2', shell=True,stderr=subprocess.STDOUT).decode('UTF-8').split('\n')[-3:-1]
    score =[x.rstrip().split() for x in score]
    Rterms=dict(zip(score[0],score[1]))
    for x in Rterms:
        Rterms[x]=float(Rterms[x])
    return(Rterms)

def calc_ProQ2(pdb_str,fasta,tmpdir,proqpath,rosetta_path):
    logging.info('Starting ProQ2 calculation')
    PATH=os.path.abspath(os.path.dirname(__file__))
    fasta_abspath=os.path.abspath(fasta)
    proq=os.path.join(PATH,'MAINEXEC','runProQ.bash')
    pdb=os.path.join(tmpdir,'input.pdb')
#    if not os.path.exists(pdb):
    with open(pdb,'w') as f:
        f.write(pdb_str)
    residues=[line for line in pdb_str.split('\n') if ' CA ' in line]
    n_residues=len(residues)
    cmd=f'cd {tmpdir};{proq} input.pdb {proqpath} {rosetta_path} {fasta_abspath} &> /dev/null;cat input.pdb.ProQ2|tail -n 1'
    score=subprocess.check_output(f'{cmd}', shell=True,stderr=subprocess.STDOUT).decode('UTF-8').split()[1]
    proq2=float(score)/n_residues
    print(score,n_residues)
    return(proq2)       

def calc_ProQDock(features,tmpdir):
    logging.info('Calculating ProQDock score')
    PATH=os.path.abspath(os.path.dirname(__file__))
    svm_model_paths=os.path.join(PATH,'SVMmodels','*.model')
    svm_classify=os.path.join(FLAGS.svm_path,'svm_classify')
    svm_input_file=os.path.join(tmpdir,'input.svm')
    feature_order={1:'rGb', 2:'nBSA', 3:'Fintres', 4:'Sc', 5:'EC', 6:'ProQ2', 7:'Isc', 8:'rTs', 9:'Erep', 10:'Etmr', 11:'CPM', 12:'Ld', 13:'CPscore'}
    svm_input=['0.0']
    for feature_no in sorted(feature_order):
        feature=feature_order[feature_no]
        if feature in features:
            #logging.info(feature)
            svm_input.append(f'{feature_no}:{features[feature]}')
    with open(svm_input_file,'w') as f:
        f.write(" ".join(svm_input))
        f.write('\n')
    preds=[]
    svm_outputs=[]
    cmds=[]
    for i,svm_model in enumerate(glob.glob(svm_model_paths)):
        #print(svm_model)
        svm_output=f'{svm_input_file}.{i}'
        cmd=f'{svm_classify} {svm_input_file} {svm_model} {svm_output} > /dev/null &'
        cmds.append(cmd)
        svm_outputs.append(svm_output)
    cmds.append('wait')
    os.system("".join(cmds))
    for svm_output in svm_outputs:
         with open(svm_output) as f:
            #print(svm_output)
            pred=f.read().rstrip().split()[0]
            #pred=subprocess.check_output(f'cat {svm_output}', shell=True,stderr=subprocess.STDOUT).decode('UTF-8').rstrip().split()[0]
            preds.append(float(pred))
    #rint(preds)
    return(np.mean(preds))

def calc_nBSA(pdb_data):
    logging.info('Calculating nBSA')
    return pdb_data['interface_area']/pdb_data['total_area']

def calc_Fintres(pdb_data):
    logging.info(f'Calculating Fintres')
    total_residues=len(set([atom.split()[-1] for atom in read_asa(pdb_data['asa_pdb_str']).keys()]))
    res_interface_A=len(set([atom.split()[-1] for atom in pdb_data['interface_A']]))
    res_interface_B=len(set([atom.split()[-1] for atom in pdb_data['interface_B']]))
    
    Fintres=(res_interface_A+res_interface_B)/total_residues
    #logging.info(f'{total_residues} {res_interface_A} {res_interface_B} {Fintres}')
    return(Fintres)


def get_quality_from_B_factor(pdb_str):
    logging.info('Calculating mean plDDT from B-factor (AF-mode)')
    plldts=[]
    for line in pdb_str.split('\n'):
        if line.startswith('ATOM') and line[12:15] == ' CA':
            #chain=line[21]
            #resnum=int(line[22:26])
            #key=f'{resnum}{chain}'
            plldt=float(line[60:66])
            plldts.append(plldt)
    return np.mean(plldts)/100

def main(argv):


    if len(argv) != 3 and not FLAGS.AF: #previous default run-mode
        print('./run_ProQDock.py <pdb> <fasta> <options>')
        print('You need to supply a pdb and fasta file or only a pdb if you like to run in --AF mode (still experimental).')
        sys.exit()
    if FLAGS.AF and len(argv)<2:
        print('./run_ProQDock.py <pdb> --AF <options>')
        print('You need to supply a pdb when running in --AF mode (still experimental).')
        sys.exit()
        
    input_pdb=argv[1]

    if FLAGS.AF:
        logging.info('Running in AF-mode (experimental)')
        fasta='AF-mode'
    else:
        fasta=argv[2]

    PATH=os.path.abspath(os.path.dirname(__file__))
    with open(os.path.join(PATH,'HELP','proqdock.ascii')) as f:
        proqdock_ascii=f.read()

    rosetta_path=os.path.join(FLAGS.rosetta,'source','bin')
    proqscorepath=rosetta_path
    rosetta_db=os.path.join(FLAGS.rosetta,'database')
    
    logging.info(f'Reading pdb: {input_pdb}')
    pdb_data=read_pdb(input_pdb)
    
    with open('AFhis2.pdb','w') as f:
        f.write(pdb_data['pdb_str'])
   # sys.exit()
    chains=sorted(pdb_data['pdb_chains'].keys())
    logging.info(f'Found {len(pdb_data["interface_A"])+len(pdb_data["interface_B"])} interface residues')
    logging.info(f'Chain {chains[0]} buries {pdb_data["interface_A_area"]:.2f}A^2 in the complex')
    logging.info(f'Chain {chains[1]} buries {pdb_data["interface_B_area"]:.2f}A^2 in the complex')
    logging.info(f'Total surface area {pdb_data["total_area"]:.2f}A^2')
    logging.info(f'Average interface area {pdb_data["interface_area"]:.2f}A^2')
    

    features={}
    with tempfile.TemporaryDirectory() as tmpdir:
        features['EC']=calc_EC(pdb_data,tmpdir,delphi_path=FLAGS.delphi_path,diel=FLAGS.diel,gauss_delphi=FLAGS.gauss)
        features['Sc']=calc_Sc(pdb_data,tmpdir,FLAGS.sc_path)
#        features['Sc']=calc_Sc(pdb_data,'./',FLAGS.sc_path)
        features['rGb']=calc_rGb(pdb_data)
        features['Ld']=calc_Ld(pdb_data['pdb_str'],tmpdir)
        features['nBSA']=calc_nBSA(pdb_data) 
        features['Fintres']=calc_Fintres(pdb_data)
        features['CPscore']=calc_CPscore(pdb_data,tmpdir)
        features['CPM']=calc_CPM(features['Sc'],features['EC'],features['nBSA'])
        Rterms=calc_rosetta_terms(pdb_data['pdb_str'],tmpdir,rosetta_path,rosetta_db)
        features.update(Rterms)

        if FLAGS.AF:
            features['ProQ2']=get_quality_from_B_factor(pdb_data['pdb_str']) #average plldt
        else:
            features['ProQ2']=calc_ProQ2(pdb_data['pdb_str'],fasta,tmpdir,FLAGS.proqpath,rosetta_path)

        ProQDock=calc_ProQDock(features,tmpdir)

        with open(os.path.join(PATH,'HELP','features.description')) as f:
            desc=f.read()
        sys.stdout.write(proqdock_ascii)
        sys.stdout.write(desc)
        for feature in features:
            if feature=='ProQ2' and FLAGS.AF:
                print(f"ProQ2 (plDDT)={features[feature]:.3f}")
            else:
                print(f"{feature}={features[feature]:.3f}")

        print('==========================') 
        print(f'ProQDock={ProQDock:.3f}')

        


              
   


if __name__ == '__main__':
    app.run(main)
