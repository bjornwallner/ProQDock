#!/usr/bin/perl

$resf = $ARGV[0];
chomp $resf;

open (INP1,"<$resf");
open (INP2,"<bury.out");

@dat1 = <INP1>;
@dat2 = <INP2>;

$rn1 = int(substr($dat1[0],0,3));

foreach $k (@dat2)
{
chomp $k;
$rn = substr($k,7,3);
$rest = substr($k,10,36);
$rnn = $rn + ($rn1-1);
printf "%7s%3d%36s\n",'       ',$rnn,$rest;
}
