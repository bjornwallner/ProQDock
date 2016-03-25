#!/usr/bin/perl

open (INP,"<ccp.out");
@dat = <INP>;

foreach $k (@dat)
{
chomp $k;
	if (substr($k,4,3) ne "GLY")
	{
	print $k,"\n";
	}
}
