#!/usr/bin/perl

# convert matrix to columns to svm format for contpref


$inpfile = $ARGV[0];	# *.contpref20
chomp $inpfile;

$outfile = $inpfile;
$outfile =~ s/\.contpref20//;
$outfile = $outfile.'-C0.svm1';

#print "$outfile\n";

open (INP,"<$inpfile");
@dat2 = <INP>;

open (OUT,">$outfile");

$m1 = sprintf("%5.3f",rand);
@coll = ();
for $c (0..scalar(@dat2)-1)
{
    chomp $dat2[$c];
    #print $dat2[$c],"\n";
    $dat2[$c] = ' '.$dat2[$c];			# For Negative entries in the first column
    @h1 = split(/\s+/,$dat2[$c]);
    for $n (($c+1)..scalar(@h1)-1)
    {
	@coll = (@coll,$h1[$n]);
	#print $h1[$n]," ";
    }
    #print "\n";
}
$lc = @coll;
#print "$sdir    $fconmat    $m1    $sflag    $lc\n";
$str = join('  ',@coll);
print $str."\n";
#print "$s1   $str  $m1   $sflag\n";
#print "====================$str===============\n";

@coll = split(/\s+/,$str);
#print scalar(@coll),"\n";
#====================================================================================================

print OUT $m1," ";

for $a (0..scalar(@coll)-1)
{
$n=$a+1;
print OUT " $n:$coll[$a] ";
}

print OUT "\n";


