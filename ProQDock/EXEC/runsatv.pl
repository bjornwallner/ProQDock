#!/usr/bin/perl

$resf = $ARGV[0];	# .res file

open (INP,"<$resf");
@dat = <INP>;

open (PRINT,">out.satv");

$l = @dat;
print $l,"\n";

for $i (0..$l-1)
{
chomp $dat[$i];
open (OUT,">target.res");
print OUT $dat[$i],"\n";
#print $dat[$i],"\n";
close OUT;
`rm -f sucal1.out`;
`./EXEC/satvp.exe`;
open (INP2,"<sucal1.out");
@store = <INP2>;
	foreach $k (@store)
	{
	chomp $k;
	print PRINT $k,"\n";
	}
close INP2;
}

