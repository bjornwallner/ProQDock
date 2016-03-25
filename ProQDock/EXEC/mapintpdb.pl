#!/usr/bin/perl

open (INT,"<intf12.res");
open (INPMAP,"<inpmap.pdb");
open (OUT,">intmapped.pdb");

@map = <INT>;
@dat = <INPMAP>;

foreach $a (@map)
{
chomp $a;
$ires = substr($a,0,3);
$res = substr($a,4,3);
$chain = substr($a,8,1);
	foreach $b (@dat)
	{
	chomp $b;
	$res1 = substr($b,17,3);
	$chain1 = substr($b,21,1);
	$ires1 = substr($b,23,3);
		if ($ires == $ires1 && $res eq $res1 && $chain eq $chain1)
		{
		print OUT $b,"\n";
		}
	}
}




