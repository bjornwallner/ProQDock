#!/usr/bin/perl

open (INP,"<outhiscysO.pdb");
open (OUT,">hiscysNC.pdb");

@dat = <INP>;

$l = @dat;
$c = 0;

$rt1 = substr($dat[0],17,3);
$rn1 = int(substr($dat[0],23,3));
$rtl = substr($dat[$l-1],17,3);
$rnl = int(substr($dat[$l-1],23,3));

#print "$rt1  $rn1  $rtl  $rnl\n";

@aa3 = ('GLY','ALA','VAL','LEU','ILE','PHE','TYR','TRP','SER','THR','CYS','CYX','MET','ASP','GLU','ASN','GLN','LYS','ARG','PRO','HID','HIE','HIP');

@aan = ('GNN','ANN','VNN','LNN','INN','FNN','YNN','WNN','SNN','TNN','CSN','CXN','MNN','DNN','ENN','NNN','QNN','KNN','RNN','PNN','HDN','HEN','HPN');

@aac = ('GCC','ACC','VCC','LCC','ICC','FCC','YCC','WCC','SCC','TCC','CSC','CXC','MCC','DCC','ECC','NCC','QCC','KCC','RCC','PCC','HDC','HEC','HPC');

for $i (0..$l-1)
{
    chomp $dat[$i];
    $rt = substr($dat[$i],17,3);
    $rn = int(substr($dat[$i],23,3));
    if ($rn == $rn1 && $rt eq $rt1)
    {
	$dat[$i] =~ s/ HT1/ H  /;
	$dat[$i] =~ s/ HT2/ H  /;
	$dat[$i] =~ s/ HT3/ H  /;
	for $j (0..22)
	{
	    if ($rt eq $aa3[$j])
	    {
		$dat[$i] =~ s/$rt/$aan[$j]/;
	    }
	}
    }
    if ($rn == $rnl && $rt eq $rtl)
    {
	for $j (0..22)
	{
	    if ($rt eq $aa3[$j])
	    {
		$dat[$i] =~ s/$rt/$aac[$j]/;
	    }
	}
    }
    print OUT $dat[$i],"\n";
}
