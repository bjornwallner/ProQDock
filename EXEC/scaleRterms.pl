#!/usr/bin/perl
#

#       RIsc          Rts         Fatr         Frep         Ftmr
#      -8.660     -339.225    -2884.541      477.750     -816.975

#             CAPRI:             MOAL:         CAPRI+MOAL:
#
#          k      x0           k      x0         k      x0
#zr1:    3.70   -0.23        3.84   -0.19      3.85   -0.19
#zr2:    2.22   -0.46        3.42   -0.33      2.87   -0.34
#Isc:    0.23   -8.75        0.33   -8.51      0.30   -8.26
#rTs:    2.98   -0.57        3.50   -0.67      3.50   -0.62
#Eatr:   3.36   -3.69        3.66   -3.96      3.39   -3.95
#Erep:   3.65    0.79        3.95    0.62      3.98    0.60
#Etmr:   2.78   -1.24        3.46   -1.25      3.27   -1.24

$rosetta_score_file=$ARGV[0];
$Rterms = $ARGV[1];
$outfile=$ARGV[2];

$k_Isc  = 0.30;  $x0_Isc  = -8.26;
$k_rTs  = 3.50;  $x0_rTs  = -0.62;
$k_Fatr = 3.39;  $x0_Fatr = -3.95;
$k_Frep = 3.98;  $x0_Frep =  0.60;
$k_Ftmr = 3.27;  $x0_Ftmr = -1.24;

#$Rterms = $ARGV[0];	# *.Rterms
#chomp $Rterms;

#$outfile = $Rterms;
#$outfile =~ s/\.Rterms/\.Rterms_scaled/;

%scores=read_rosetta_score($rosetta_score_file);

foreach $decoy(keys(%scores)) {

   # print "$decoy $scores{$decoy}\n";
    $Isc = $scores{$decoy}{'I_sc'};
    $rTs = $scores{$decoy}{'score'};
    $Fatr = $scores{$decoy}{'fa_atr'};
    $Frep = $scores{$decoy}{'fa_rep'};
    $Ftmr = $rTs - $Frep;
    $pdb=$decoy;
    $pdb=~s/_0001$/.pdb/;
    $len=`grep -c 'CA' $pdb`;
    chomp($len);
#    print $pdb
	
}
print "LENGTH: ",$len,"\n";
$nrTs = $rTs/$len;
$nFatr = $Fatr/$len;
$nFrep = $Frep/$len;
$nFtmr = $Ftmr/$len;

$sIsc  = 1/(1+(exp(-$k_Isc*($Isc-$x0_Isc))));
$srTs  = 1/(1+(exp(-$k_rTs*($nrTs-$x0_rTs))));
$sFatr = 1/(1+(exp(-$k_Fatr*($nFatr-$x0_Fatr))));
$sFrep = 1/(1+(exp(-$k_Frep*($nFrep-$x0_Frep))));
$sFtmr = 1/(1+(exp(-$k_Ftmr*($nFtmr-$x0_Ftmr))));

open(OUT,">$outfile");
printf OUT "%10s  %10s  %10s  %10s  %10s\n",'Isc','rTs','Eatr','Erep','Etmr';
printf OUT "%10.5f  %10.5f  %10.5f  %10.5f  %10.5f\n",$sIsc,$srTs,$sFatr,$sFrep,$sFtmr;
printf "%10.3f  %10.3f  %10.3f  %10.3f  %10.3f\n",$Isc,$nrTs,$nFatr,$nFrep,$nFtmr;
close(OUT);

open(OUT2,">$Rterms"); 
printf OUT2 ("%1s %10s %12s %12s %12s %12s\n","#","RIsc","Rts","Fatr","Frep","Ftmr");
printf OUT2 ("%12.3f %12.3f %12.3f %12.3f %12.3f\n",$Isc,$rTs,$Fatr,$Frep,$Ftmr);
close(OUT2);

close(OUT2);
sub read_rosetta_score
{
    my $file=shift;
    if($file=~/\.gz/) {
	open(FILE,"gunzip -c $file|");
    } else {
	open(FILE,$file);
    }
    my @labels=();
    my %scores=();
    my %label2index=();
    while(<FILE>) 
    {
	#print;
	if(/^SCORE:.+score/) {
	    chomp;
	    @labels=split(/\s+/);
	    for(my $i=0;$i<scalar(@labels);$i++) {
		$label2index{$labels[$i]}=$i;
	    }
	} elsif(/^SCORE:/) {
	    my @temp=split(/\s+/);
	    my $decoy=$temp[$#temp];
	    for(my $i=0;$i<scalar(@labels);$i++) {
		$scores{$decoy}{$labels[$i]}=$temp[$i];
	    }

	}
    }
    close(FILE);
    return %scores;

}
