#!/usr/bin/perl

open (INP,"<chfn.inp") || die "chfn.inp not found\n";
$fn = <INP>;
chomp $fn;

#print $fn,"\n";

$f1 = 0;

if ($fn =~ m/\.pdb/)
{
    $f1 = 1;
}

$f2 = 1;

if ($fn =~ m/\.PDB/)
{
$f2 = 0;
}

@h = split(//,$fn);

$cntdot = 0;

foreach (@h)
{
	if ($_ eq ".") 
	{
	    $cntdot++;
	    #print $_
	}
}

$f3 = 0;

if ($cntdot == 1)
{
$f3 = 1;
}

#print "$f1  $f2 $f3 $cntdot\n";

if ($f1 == 1 && $f2 == 1 && $f3 == 1)
{
print "OK\n";
}
