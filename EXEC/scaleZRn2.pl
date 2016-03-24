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


$k_zr   = 3.85;  $x0_zr   = -0.19;
$k_zr2  = 2.87;  $x0_zr2  = -0.34;

$Zterms = $ARGV[0];	# *.zrankN2
chomp $Zterms;

$outfile = $Zterms;
$outfile =~ s/\.zrankN2/\.zrankN2_scaled/;

open (OUT,">$outfile");

$res = $Zterms;
$res =~ s/\.zrankN2/\.res/;

open (INP,"<$Zterms");
@dat = <INP>;

foreach $a (@dat)
{
chomp $a;
#print $a,"\n";
}

if ($dat[0] eq "#         zrank           zrank2")
{
@hold = split(/\s+/,$dat[1]);
$zr = $hold[1];
$zr2 = $hold[2];
#print "$zr  $zr2\n";
}

if (-e "$res")
{
$len=`wc -l $res | cut -f1 -d' '`;
chomp $len;
print "LENGTH FILE FOUND\n";
print "LENGTH: ",$len,"\n";
$nzr = $zr/$len;
$nzr2 = $zr2/$len;

$szr  = 1/(1+(exp(-$k_zr*($nzr-$x0_zr))));
$szr2  = 1/(1+(exp(-$k_zr2*($nzr-$x0_zr2))));
}

printf OUT "%10s  %10s\n",'zrank','zrank2';
#printf OUT "%10.3f  %10.3f\n",$nzr,$nzr2;
printf OUT "%10.5f  %10.5f\n",$szr,$szr2;


