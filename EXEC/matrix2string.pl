#!/usr/bin/perl

# convert matrix to columns to svm format for contpref


$inpfile = $ARGV[0];	# *.contpref20
open (INP,"<$inpfile");
@dat2 = <INP>;
@coll = ();
for $c (0..scalar(@dat2)-1)
{
    chomp $dat2[$c];
    #print $dat2[$c],"\n";
    $dat2[$c] = ' '.$dat2[$c];			# For Negative entries in the first column
    @h1 = split(/\s+/,$dat2[$c]);
    for $n (($c+1)..scalar(@h1)-1)
    {
	if($h1[$n] == 0) {
	    @coll = (@coll,0);
	} else {
	    $val=$h1[$n];
	    $val=~s/0+$//;
	    #print "$val $h1[$n]";
	    @coll = (@coll,$val); 
	}
	#print $h1[$n]," ";
    }
    #print "\n";
}
#print "$sdir    $fconmat    $m1    $sflag    $lc\n";
$str = join('  ',@coll);
print $str."\n";
 
