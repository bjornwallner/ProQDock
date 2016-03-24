#!/usr/bin/perl

#====================================================================================
#  Check compatibility of the input file
#------------------------------------------------------------------------------
#  It should contain two and exactly two polypeptide chains
#====================================================================================

$flagUlt=0;

open (OUTF,">formch.out");

$inppdb = $ARGV[0] || die "Enter PDB file\n";
chomp $inppdb;
open (PDBFILE,"<$inppdb");
@fdat = <PDBFILE>;

print "\n\nINPUT PDB : $inppdb\n\n";

@pcoords = grep(/^ATOM\s/,@fdat);
%h = ();

foreach $k (@pcoords)
{
chomp $k;
$h{substr($k,21,1)}++;
}

@chain = keys %h;
$nch = @chain;
print $nch,"\n";

if ($nch == 2)
{
print "\n\nThe input pdb file contains \'TWO\' polypeptide chains ($chain[0] & $chain[1]) as required. \n\n";
}
else
{
$flagUlt=1;
print "=======================================================================================================\n";
print "\n\nThe input pdb file does not contain exactly \'TWO\' polypeptide chains and thus not acceptable for calculation \n [Rather it contains $nch chains]\n";
	foreach $c (@chain)
	{
	print $c,"\n";
	}
print "\nProgram will exit\n";
print "It should contain \'TWO and exactly TWO\' polypeptide chains\n\n";
print "=======================================================================================================\n";
#exit;
}

print OUTF "FLAG: $flagUlt\n";
print "FLAG: $flagUlt\n";

