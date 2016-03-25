#!/usr/bin/perl

$pdb = $ARGV[0];
chomp $pdb;

open (INP1,"<$pdb");
@dat1 = <INP1>;
close INP1;

foreach $a (@dat1)
{
chomp $a;
$atom = substr($a,13,1);
	if ($atom ne "H")
	{
	print $a,"\n";
	}
}


