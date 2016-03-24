#!/usr/bin/perl

`rm -f ccp.out`;
`rm -f sf.log`;
`rm -f cf.log`;
`rm -f sf.pot`;
`rm -f cf.pot`;
`rm -f sout.pdb`;
`rm -f cout.pdb`;

$resf = $ARGV[0];		# *-m.res
chomp $resf;

$dir = $ARGV[1];		# directory name e.g., dir2haq
chomp $dir;

$gind = $ARGV[2];		# globular index (cutoff: 9.5 Angstrom)
chomp $gind;

$buryf = $ARGV[3];		# bury file
chomp $buryf;

open (OUT,">>ccp.out");

open (INP,"<$resf");

open (BUR,"<$buryf") || print "Bury file not found : $buryf\n";
@bury = <BUR>;
foreach (@bury) {chomp $_;}

@dat = <INP>;

@aa3 = ('GLY','ALA','VAL','LEU','ILE','PHE','TYR','TRP','SER','THR','CYS','CYX','MET','ASP','GLU','ASN','GLN','LYS','ARG','PRO','HID','HIE','HIP','GNN','ANN','VNN','LNN','INN','FNN','YNN','WNN','SNN','TNN','CSN','CXN','MNN','DNN','ENN','NNN','QNN','KNN','RNN','PNN','HDN','HEN','HPN','GCC','ACC','VCC','LCC','ICC','FCC','YCC','WCC','SCC','TCC','CSC','CXC','MCC','DCC','ECC','NCC','QCC','KCC','RCC','PCC','HDC','HEC','HPC');

@aa1 = ('G','A','V','L','I','F','Y','W','S','T','C','C','M','D','E','N','Q','K','R','P','H','H','H','G','A','V','L','I','F','Y','W','S','T','C','C','M','D','E','N','Q','K','R','P','H','H','H','G','A','V','L','I','F','Y','W','S','T','C','C','M','D','E','N','Q','K','R','P','H','H','H');

foreach (@aa3){chomp $_;}
foreach (@aa1){chomp $_;}

foreach $k (@dat)
{
chomp $k;
$rn = int(substr($k,0,3));
$res = substr($k,4,3);
$chn = substr($k,8,1);
$flag = 0;
	foreach $rs (@aa3)
	{
		if ($res eq $rs)
		{
		$flag = 1;
		}
	}
$bur = 1.00;
	foreach $b (@bury)
	{
		if (int(substr($b,7,3)) == $rn)
		{
		$bur = substr($b,42,4);
		}
	}
	for $i (0..scalar(@aa3)-1)
	{
		if ($res eq $aa3[$i])
		{
		$rt1 = $aa1[$i];
		}
	}
	if ($flag == 1 && $res ne "GLY")
	{
	$self_field = $dir.'/'.$rt1.$rn.$chn.'sf.pdb'; 	# field input 1
	$comp_field = $dir.'/'.$rt1.$rn.$chn.'cf.pdb';	# field input 2
	$surf_grid = $dir.'/'.$rt1.$rn.$chn.'s.pdb';	# surface grid input 1,2
#	$modpdb1 = $rt1.$rn.'sout.pdb';		# charge assigned pdb output 1
	$modpdb1 = 'sout.pdb';			# charge assigned pdb output 1
#	$modpdb2 = $rt1.$rn.'cout.pdb';		# charge assigned pdb output 2
	$modpdb2 = 'cout.pdb';			# charge assigned pdb output 2
#	$pot1 = $rt1.$rn.$chn.'s.pot';		# potential on surface output 1
	$pot1 = 'sf.pot';			# potential on surface output 1
#	$pot2 = $rt1.$rn.$chn.'c.pot';		# potential on surface output 2
	$pot2 = 'cf.pot';			# potential on surface output 2
	$log1 = 'sf.log';			# log out 1 (will be overwritten)
	$log2 = 'cf.log';			# log out 2 (will be overwritten)
#printf "%15s  %15s  %15s  %15s  %15s  %15s  %15s\n",$self_field,$comp_field,$surf_grid,$pot1,$pot2,$log1,$log2;
	`./EXEC/generateprm.pl $self_field $surf_grid $modpdb1 $pot1 $gind`;
	`mv script.prm script1.prm`;
	`./EXEC/generateprm.pl $comp_field $surf_grid $modpdb2 $pot2 $gind`;
	`mv script.prm script2.prm`;
	`delphi_static script1.prm > $log1`;
	`delphi_static script2.prm > $log2`;
        `./EXEC/extpot.pl $pot1 > temp1.pot`;
        `./EXEC/extpot.pl $pot2 > temp2.pot`;
        $out = `./EXEC/ccpsw.exe temp1.pot temp2.pot`;
	chomp $out;
#	chomp $out2;
#		if ($out != $out2)
#		{
#		print "Problem with header lines in $pot1 & $pot2\n";
#		exit;
#		}        
	printf OUT "%3d%1s%3s     %4.2f     %24s %1s\n",$rn,'-',$res,$bur,$out,$chn;
	}
}


