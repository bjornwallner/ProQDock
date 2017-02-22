#!/usr/bin/perl

# convert matrix to columns to svm format for contpref


$inpfile = $ARGV[0];	# *.contpref20
open (INP,"<$inpfile");
@dat2 = <INP>;
@coll = ();
$sum_total=0;
@sums=();
for $c (0..scalar(@dat2)-1)
{
    chomp $dat2[$c];
    #print $dat2[$c],"\n";
    $dat2[$c] = ' '.$dat2[$c];			# For Negative entries in the first column
    @h1 = split(/\s+/,$dat2[$c]);
    $sumrow=0;
    for $n (0..scalar(@h1)-1)
    {
	$sumrow=$sumrow+$h1[$n];
    }
    $sum_total=$sum_total+$sumrow;
    push(@sums,$sumrow);
    #print "\n";
}
#foreach $s(@sums) {
#    printf("%-8s ",$s);
#}
#print "\n";

foreach $s(@sums) {
    $frac=$s/$sum_total;
    printf("%-8.3f ",$frac);
}
print "\n";

 
