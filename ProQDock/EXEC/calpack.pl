#!/usr/bin/perl

$ifile = $ARGV[0];	# *.Sm
chomp $ifile;

open (INP,"<$ifile");
@dat1 = <INP>;
close INP;

open (LIB1,"<Smbur1.probdist");
open (LIB2,"<Smbur2.probdist");
open (LIB3,"<Smbur3.probdist");

@lib1 = <LIB1>;
@lib2 = <LIB2>;
@lib3 = <LIB3>;

$l1 = @lib1;
$l2 = @lib2;
$l3 = @lib3;

$c1 = 0;
$c2 = 0;
$c3 = 0;

$Pscore1 = 0.00;
$Pscore2 = 0.00;

$cnt1 = 0.00;
$cnt2 = 0.00;

foreach $k (@dat1)
{
chomp $k;
$burial = substr($k,9,4);
$Smsc = substr($k,25,6);
#print "$burial  $Smsc\n";
	if ($burial >= 0.00 && $burial <= 0.05)
	{
	$c1++;
		for $i (0..$l1-1)
		{
		chomp $lib1[$i];
		$Smstart = substr($lib1[$i],0,5);
		$Smstop = substr($lib1[$i],7,5);
		$prob = substr($lib1[$i],21,5);
			if ($Smsc >= $Smstart && $Smsc <= $Smstop)
			{
#			print "$burial  $Smsc  $prob\n";
			$Pscore1 = $Pscore1 + $prob;
			$cnt1++;
				if ($prob == 0.00)
				{
				goto SKIP;
				}
			$Pscore2 = $Pscore2 + (log($prob)/log(10));
			$cnt2++;
			}
		}
	}
	if ($burial > 0.05 && $burial <= 0.15)
	{
	$c2++;
		for $i (0..$l2-1)
		{
		chomp $lib2[$i];
		$Smstart = substr($lib2[$i],0,5);
		$Smstop = substr($lib2[$i],7,5);
		$prob = substr($lib2[$i],21,5);
			if ($Smsc >= $Smstart && $Smsc <= $Smstop)
			{
#			print "$burial  $Smsc  $prob\n";
			$Pscore1 = $Pscore1 + $prob;
			$cnt1++;
				if ($prob == 0.00)
				{
				goto SKIP;
				}
			$Pscore2 = $Pscore2 + (log($prob)/log(10));
			$cnt2++;
			}
		}
	}
	if ($burial > 0.15 && $burial <= 0.30)
	{
	$c3++;
		for $i (0..$l3-1)
		{
		chomp $lib3[$i];
		$Smstart = substr($lib3[$i],0,5);
		$Smstop = substr($lib3[$i],7,5);
		$prob = substr($lib3[$i],21,5);
			if ($Smsc >= $Smstart && $Smsc <= $Smstop)
			{
#			print "$burial  $Smsc  $prob\n";
			$Pscore1 = $Pscore1 + $prob;
			$cnt1++;
				if ($prob == 0.00)
				{
				goto SKIP;
				}
			$Pscore2 = $Pscore2 + (log($prob)/log(10));
			$cnt2++;
			}
		}
	}
SKIP:
}

$tot = $c1 + $c2 + $c3;

$Avgscore = $Pscore1/$cnt1;
$Avglog = $Pscore2/$cnt2;

printf "%8.3f\n",$Avglog;

#print $tot,"\n";

