#!/usr/bin/perl

open (INPSURF,"<inp12map-surf.pdb");

@dat = <INPSURF>;

open (MAP,"<intf12.res");
open (OUT,">intf12-surf.pdb");

@map = <MAP>;

foreach $a (@map)
{
chomp $a;
#print $a,"\n";
$ires = substr($a,0,3);
$res = substr($a,4,3);
$chain = substr($a,8,1);
#print "$ires  $res  $chain\n";
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

