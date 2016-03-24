#!/usr/bin/perl

$saatog = $ARGV[0];	# saa.out for the complex as a whole (unmapped)
chomp $saatog;

open (INP,"<$saatog");
@saadat = <INP>;

$pdb1 = $ARGV[1];	# pdb1 (unmapped)
chomp $pdb1;

$pdb2 = $ARGV[2];	# pdb2 (unmapped)
chomp $pdb2;

open (PDB1,"<$pdb1");
open (PDB2,"<$pdb2");

$burmapf = $saatog;
$burmapf =~ s/\.saa/\.bury/;

$resf1 = $pdb1;
$resf1 =~ s/\.pdb/\.res/;

$resf2 = $pdb2;
$resf2 =~ s/\.pdb/\.res/;

open (RES1,"<$resf1") || die "$resf1 not found\n";
open (RES2,"<$resf2") || die "$resf2 not found\n";
open (OUTS,">saamap.out");

@rinp1 = <RES1>;
@rinp2 = <RES2>;

@restog = (@rinp1,@rinp2);

for $a (0..scalar(@restog)-1)
{
chomp $restog[$a];
$rid = substr($restog[$a],4,5);
$ires = substr($restog[$a],0,3);
$res = substr($restog[$a],4,3);
$chain = substr($restog[$a],8,1);
$rn = sprintf("%3d",$a+1);
$rnew = $rn.'-'.$rid;
#print "$restog[$a] -> $rnew\n";
	foreach $s (@saadat)
	{
	chomp $s;
	$len = length($s);
        $l2 = $len - 18;
	$ires1 = substr($s,15,3);
	$chain1 = substr($s,19,1);
	$res1 = substr($s,22,3);
	$nstr = substr($s,0,15).sprintf("%3d",$rn).substr($s,18,$l2);
		if (($ires == $ires1) && ($res eq $res1) && ($chain eq $chain1))
		{
		print OUTS $nstr,"\n";
		}
	}
}

`cp saamap.out saa.out`;
`rm bury.out`;
`rm bury.scrt`;
`./bury.exe`;
`cp bury.out $burmapf`;


