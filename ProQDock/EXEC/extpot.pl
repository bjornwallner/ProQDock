#!/usr/bin/perl

$file1 = $ARGV[0];	# sf.pot / cf.pot

open (INP1,"<$file1");

@dat1 = <INP1>;

$l1 = @dat1;

for $i (0..$l1-1)
{
chomp $dat1[$i];
	if (@dat1[$i] =~ m/^ATOM DESCRIPTOR\s+/)
	{
	$start = $i+1;
	}
	if (@dat1[$i] =~ m/^ total energy =\s+/)
	{
	$stop = $i-1;
	}
}

for $i ($start..$stop)
{
print $dat1[$i],"\n";
}





