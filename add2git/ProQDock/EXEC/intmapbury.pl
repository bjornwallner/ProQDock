#!/usr/bin/perl

$file1 = $ARGV[0];	# bury.out for the complex (mapped from res1)

open (INP1,"<$file1");

@dat = <INP1>;

open (MAP,"<intf12.res");
open (OUT,">intmapped.bury");

@map = <MAP>;

foreach $a (@map)
{
chomp $a;
#print $a,"\n";
$ires = substr($a,0,3);
$res = substr($a,4,3);
$chain = substr($a,8,1);
	foreach $b (@dat)
	{
	chomp $b;
	$res1 = substr($b,15,3);
	$chain1 = substr($b,19,1);
	$ires1 = substr($b,7,3);
		if (($ires == $ires1) && ($res eq $res1) && ($chain eq $chain1))
		{
		print OUT $b,"\n";
		}
	}
}




