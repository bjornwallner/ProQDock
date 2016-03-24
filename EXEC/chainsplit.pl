#!/usr/bin/perl

$pdb = $ARGV[0];
chomp $pdb;
open (INP,"<$pdb");
@dat = <INP>;

%hash = ();

@atoms = grep (/^ATOM\s+/,@dat);
@hetatms = grep (/^HETATM\s+/,@dat);

foreach (@atoms) {chomp $_;}
foreach (@hetatms) {chomp $_;}

$chain1 = substr($atoms[0],21,1);
$chain2 = substr($atoms[scalar(@atoms)-1],21,1);

print "$chain1~$chain2\n";

open (OUT1,">inp1.pdb");
open (OUT2,">inp2.pdb");

foreach $b (@atoms)
{
$ch = substr($b,21,1);
	if ($ch eq $chain1)
	{
	print OUT1 $b,"\n";
	}
	if ($ch eq $chain2)
	{
	print OUT2 $b,"\n";
	}
}

foreach $b (@hetatms)
{
$ch = substr($b,21,1);
	if ($ch eq $chain1)
	{
	print OUT1 $b,"\n";
	}
	if ($ch eq $chain2)
	{
	print OUT2 $b,"\n";
	}
}


