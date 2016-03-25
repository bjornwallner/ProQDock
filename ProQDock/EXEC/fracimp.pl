#!/usr/bin/perl

$file = $ARGV[0];	# *.cnt
chomp $file;

open (INP,"<$file");
@dat = <INP>;

@imp = grep (/improbable/,@dat);

$li = @imp;
$tot = @dat;

$frac = 100*($li/$tot);

$status = 'p';

if ($frac >= 15.0)
{
$status = 'f';
}

#printf "%4s  %3d  %3d  %1s\n",substr($file,0,4),$li,$tot,$status;
printf "%8.3f\n",$frac;

