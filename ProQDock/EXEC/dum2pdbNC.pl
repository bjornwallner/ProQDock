#!/usr/bin/perl

# replace selected atoms by dummy in a modified pdb file
# run twice with proper res.replace to generate (1) respdb (field due to the residue alone) & (2) rescomp pdb (field due to the rest of the protein)
# Include modified N-C terminal cases : total cases : 69 

@rest = ('GLY','ALA','VAL','LEU','ILE','PHE','TYR','TRP','SER','THR','CYS','CYX','MET','ASP','GLU','ASN','GLN','LYS','ARG','PRO','HID','HIE','HIP','GNN','ANN','VNN','LNN','INN','FNN','YNN','WNN','SNN','TNN','CSN','CXN','MNN','DNN','ENN','NNN','QNN','KNN','RNN','PNN','HDN','HEN','HPN','GCC','ACC','VCC','LCC','ICC','FCC','YCC','WCC','SCC','TCC','CSC','CXC','MCC','DCC','ECC','NCC','QCC','KCC','RCC','PCC','HDC','HEC','HPC','SOD','MAG','ALM','POT','CAL','CRM','MNG','IRN','COB','NIC','COP','ZNC','SLV','CDM','PLT','GLD','MRC');

@dumm = ('GDD','ADD','VDD','LDD','IDD','FDD','YDD','WDD','SDD','TDD','CSD','CXD','MDD','DDD','EDD','NDD','QDD','KDD','RDD','PDD','HDD','HED','HPD','GDD','ADD','VDD','LDD','IDD','FDD','YDD','WDD','SDD','TDD','CSD','CXD','MDD','DDD','EDD','NDD','QDD','KDD','RDD','PDD','HDD','HED','HPD','GDD','ADD','VDD','LDD','IDD','FDD','YDD','WDD','SDD','TDD','CSD','CXD','MDD','DDD','EDD','NDD','QDD','KDD','RDD','PDD','HDD','HED','HPD','DSO','DMG','DAL','DPT','DCA','DCR','DMN','DIR','DCO','DNI','DCU','DZN','DSL','DCD','DPL','DGL','DMR');

$l1 = @rest;
$l2 = @dumm;

#print "$l1  $l2\n";

#$pdb = $ARGV[0] || die "PDB file does not exist\n";	# *.pdb
#chomp $pdb;

open (PDB,"<inp-m.pdb");
open (REP,"<res.replace");

@dat = <PDB>;

@repl = <REP>;
foreach (@repl){chomp $_;}#print $_,"\n";}

open (OUT1,">dumm.pdb");    

foreach $k (@dat)
{
chomp $k;
$res = substr($k,17,3);
$chn = substr($k,21,1);
$rno = int(substr($k,23,3));
$flag = 0;
$c = 0;
$modres = $res;		# default
	foreach $r (@repl)
	{
	$rs = substr($r,4,3);
	$rn = int(substr($r,0,3));
	$ch = substr($r,8,1);
		if ($rno == $rn && $res eq $rs && $chn eq $ch)
		{
		$c++;
		}
	}
	if ($c == 1)
	{
	# requires to be replaced by suitable dummy
	$flag = 1;
	}
	if ($flag == 1)
	{
		for $i (0..scalar(@rest)-1)
		{
			if ($res eq $rest[$i])
			{
			$modres = $dumm[$i];
			}
		}
	}
$str = substr($k,0,17).$modres.substr($k,20, );
print OUT1 $str,"\n";
}
close OUT1;
