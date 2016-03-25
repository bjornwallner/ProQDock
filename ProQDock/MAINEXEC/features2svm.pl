#!/usr/bin/perl

$file = $ARGV[0];	# Raw .SVMfeatures file containing all columns
chomp $file;

open (INP,"<$file");
@dat = <INP>;

$header = $dat[0];
$scores = $dat[1];

chomp $header;
chomp $scores;

$flag = 0;

if ($header eq "SCORE       rGb      nBSA   Fintres        Sc        EC      ProQ       Isc       rTs      Erep      Etmr       CPM        Ld   CPscore")
{
#print $scores,"\n";
$flag = 1;
print "Correct Header meaning correct order of features: Program will proceed\n";
}
else
{
die "Incorrect Input file: Program will exit\n";
}

if ($flag == 1)
{
goto Further;
}

Further:

$outfile = $file.'.svm';
open (OUT,">$outfile");

$header =~ s/^SCORE//;
#print $header,"\n";

$scores =~ s/^SCORE//;
#print $scores,"\n";

#======================================================================

@hold_header = split(/\s+/,$header);
$l1 = @hold_header;
@hold_scores = split(/\s+/,$scores);
$l2 = @hold_scores;
$n1 = 0;
$DockQ = 'DockQ';
printf OUT "%1s%7s  ",'#','DockQ'; 			
	for $b (1..$l1-1)
	{
	$n1++;
	print OUT "$n1:$hold_header[$b]  ";
	}
print OUT "\n";

#======================================================================

$n1 = 0;
$DockQ = rand;
printf OUT "%8.3f  ", $DockQ; 			# Random & Arbitary
	for $b (1..$l2-1)
	{
	$n1++;
	printf OUT "$n1:$hold_scores[$b]  ";
	}
print OUT "\n";

#======================================================================

