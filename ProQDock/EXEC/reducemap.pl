#!/usr/bin/perl

$file = $ARGV[0];
chomp $file;

open (INP,"<$file");
@dat = <INP>;

foreach $k (@dat)
{
chomp $k;
$pre = substr($k,0,12);
$atom = substr($k,12,4);
$pro = substr($k,16, );
	if (substr($atom,0,1) eq "H" || substr($atom,1,1) eq "H")	# Hydrogens
	{			
		if (substr($atom,1,1) eq "H" && substr($atom,0,1) ne "H")
		{
		# correct format
		print $k,"\n";
		}
		elsif (substr($atom,0,1) eq "H")
		{
		# Requeres reordering (to be consistant with reduce version 2.15)
		$reorder_atom = substr($atom,3,1).substr($atom,0,3);
#		print "$atom  $reorder_atom\n";
		$nline = $pre.$reorder_atom.$pro;
		print "$nline\n";
		}
	}
	else								# Non-hydrogens
	{
	print $k,"\n";
	}
}

