#!/usr/bin/perl

$pdb = $ARGV[0];
chomp $pdb;

open (INP1,"<$pdb");
@dat1 = <INP1>;
close INP1;

foreach $a (@dat1)
{
chomp $a;
$atom1 = substr($a,12,1);
$atom2 = substr($a,13,1);
	if ($atom1 ne "H" && $atom2 ne "H")
	{
	print $a,"\n";
	}
}


