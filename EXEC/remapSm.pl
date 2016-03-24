#!/usr/bin/perl

$resf = $ARGV[0];
chomp $resf;

open (INP1,"<$resf");
open (INP2,"<out.satv");

@dat1 = <INP1>;
@dat2 = <INP2>;

$rn1 = int(substr($dat1[0],0,3));

foreach $k (@dat2)
{
chomp $k;
$rn = int(substr($k,0,3));
$rest = substr($k,3,37);
$rnn = $rn + ($rn1-1);
printf "%3d%37s\n",$rnn,$rest;
}
