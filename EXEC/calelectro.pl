#!/usr/bin/perl

$ifile = $ARGV[0];	# *.Em
chomp $ifile;

open (INP,"<$ifile");
@dat1 = <INP>;
close INP;

open (LIB1,"<Embur1.probdist");
open (LIB2,"<Embur2.probdist");
open (LIB3,"<Embur3.probdist");

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
$burial = substr($k,12,4);
$Emsc = substr($k,31,6);
#print "$burial  $Smsc\n";
	if ($burial >= 0.00 && $burial <= 0.05)
	{
	$c1++;
		for $i (0..$l1-1)
		{
		chomp $lib1[$i];
		$Emstart = substr($lib1[$i],0,5);
		$Emstop = substr($lib1[$i],7,5);
		$prob = substr($lib1[$i],21,5);
			if ($Emsc >= $Emstart && $Emsc <= $Emstop)
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
		$Emstart = substr($lib2[$i],0,5);
		$Emstop = substr($lib2[$i],7,5);
		$prob = substr($lib2[$i],21,5);
			if ($Emsc >= $Emstart && $Emsc <= $Emstop)
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
		$Emstart = substr($lib3[$i],0,5);
		$Emstop = substr($lib3[$i],7,5);
		$prob = substr($lib3[$i],21,5);
			if ($Emsc >= $Emstart && $Emsc <= $Emstop)
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

