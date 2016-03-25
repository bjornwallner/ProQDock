#!/usr/bin/perl

$fn = $ARGV[0];		# $code.res
chomp $fn;
open (INP, "<$fn");
@res = <INP>;

open (INP2,"<surfinp.pdb");
@surf = <INP2>;

@atoms = grep(/^ATOM\s+/,@surf);

$l = @atoms;

$rn1 = int(substr($res[0],0,3));

for $i (0..$l-1)
{
chomp $atoms[$i];
$fp = substr($atoms[$i],0,22);
$rn = int(substr($atoms[$i],23,3));
$sp = substr($atoms[$i],26, );
$rn = $rn + ($rn1-1);
printf "%22s%4d%28s\n",$fp,$rn,$sp;
}
