#!/usr/bin/perl

open (INP1,"<intf12.res");
open (INP2,"<inp12map-m.res");
open (OUT,">intf12-m.res");

@dat1 = <INP1>;
@dat2 = <INP2>;

foreach $a (@dat1)
{
chomp $a;
$rn = substr($a,0,3);
$res = substr($a,4,3);
$ch = substr($a,8,1);
	foreach $b (@dat2)
	{
	chomp $b;
	$rn1 = substr($b,0,3);
	$rt1 = substr($b,4,3);
	$ch1 = substr($b,8,1);
		if ($rn == $rn1 && $ch eq $ch1)
		{
		$res = $rt1;
		}
	}
printf OUT "%3d%1s%3s%1s%1s\n",$rn,'-',$res,' ',$ch;
}
