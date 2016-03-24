#!/usr/bin/perl

$file = $ARGV[0];
chomp $file;

open (INP,"<$file");
@dat = <INP>;
@atoms = grep(/^ATOM\s+/,@dat);

chomp $dat[0];
$chain1 = substr($atoms[0],21,1);

$rnc = 0;

for $i (0..scalar(@atoms)-1)
{
chomp $atoms[$i];
$ires = int(substr($atoms[$i],22,4));
$ch = substr($atoms[$i],21,1);
	if ($ch eq $chain1)
	{
	$rnc = $ires;
	}
}

print $rnc,"\n";

