#!/usr/bin/perl

open (INP1,"<orig.res") || die "orig.res not found\n";
@dat1 = <INP1>;

open (INP2,"<inp12map.res") || die "inp12map.res not found\n";
@dat2 = <INP2>;

open (OUT,">reso2n.map");

$l1 = @dat1;
$l2 = @dat2;

if ($l1 != $l2)
{
die "LENGTH MISMATCH BETWEEN THE ORIGINAL AND THE CONVERTED RES FILES\n";
}
else
{
	for $i (0..$l1-1)
	{
	chomp $dat1[$i];
	chomp $dat2[$i];
	printf OUT "%9s %2s %9s\n",$dat1[$i],'->',$dat2[$i];
	}
}




