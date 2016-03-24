#!/usr/bin/perl

$a1 = $ARGV[0];
chomp $a1;
$a2 = $ARGV[1];
chomp $a2;

$mean = ($a1+$a2)/2;
print $mean,"\n";
