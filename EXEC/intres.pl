#!/usr/bin/perl

$fort14 = $ARGV[0] || die "Enter delasa output for interface 1 (fort.14) or equivalent\n"; # fort.14
$fort15 = $ARGV[1] || die "Enter delasa output for interface 2 (fort.15) or equivalent\n"; # fort.15

if (scalar(@ARGV) < 2)
{
die "Enter delasa output for interface1 (fort.14) and interface2 (fort.15) or equivalent files\n\n";
}

open (INP1,"<$fort14");
open (INP2,"<$fort15");
open (MAP,"<reso2n.map");

open (OUT1,">intf1.res");
open (OUT2,">intf2.res");
open (RAS1,">ras1.select");
open (RAS2,">ras2.select");

@dat1 = <INP1>;
@dat2 = <INP2>;
@map = <MAP>;

delete $dat1[0];
delete $dat2[0];

@ir1 = ();
@ir2 = ();

$chain1 = substr($dat1[1],15,1);
$chain2 = substr($dat2[1],15,1);

print "$chain1\n";
print "$chain2\n";
 
for $a (1..scalar(@dat1)-1)
{
chomp $dat1[$a];
$ires1 = int(substr($dat1[$a],9,4)).'-'.substr($dat1[$a],5,3).'-'.substr($dat1[$a],15,1);
@ir1 = (@ir1,$ires1);
#print $ires1,"\n";
}

for $a (1..scalar(@dat2)-1)
{
chomp $dat2[$a];
$ires2 = int(substr($dat2[$a],9,4)).'-'.substr($dat2[$a],5,3).'-'.substr($dat2[$a],15,1);
@ir2 = (@ir2,$ires2);
#print $ires2,"\n";
}

%h1 = ();
%h2 = ();

foreach $a (@ir1)
{
$h1{$a}++;
}

foreach $a (@ir2)
{
$h2{$a}++;
}

@ir1u = sort {$a <=> $b} keys %h1;
@ir2u = sort {$a <=> $b} keys %h2;

$i1c = @ir1u;
$i2c = @ir2u;

print "==========================================================================================\n";
print "Number of Interfacial Residues buried upon complexation: \n";
print "CHAIN: $chain1 : $i1c\n"; 
print "CHAIN: $chain2 : $i2c\n";
print "==========================================================================================\n";

#print OUT1 "Interfacial Residues for Mol1\n";

$rasstr1 = 'select ';
$rasstr2 = 'select ';

foreach $a (@ir1u)
{
@ar1 = split(/-/,$a);
	foreach $m (@map)
	{
	chomp $m;
	$irold = substr($m,0,4);
	$irnew = substr($m,14,4);
	$res = substr($m,5,3);
	$chain = substr($m,9,1);
		if (($ar1[0] == $irold) && ($ar1[1] eq $res) && ($ar1[2] eq $chain))
		{
		$ar1[0] = $irnew;
		printf OUT1 "%3d%1s%3s %1s\n",$ar1[0],'-',$ar1[1],$ar1[2]; 
		printf RAS1 "%14s %3d\n","select :$chain1 and ",$ar1[0];
#		$rasstr1 .= sprintf("%8s%3d%2s"," :$chain1 and ",$ar1[0],"\, "); 
		}
	}
}

#print OUT2 "Interfacial Residues for Mol2\n";

foreach $a (@ir2u)
{
@ar2 = split(/-/,$a);
	foreach $m (@map)
	{
	chomp $m;
	$irold = substr($m,0,4);
	$irnew = substr($m,14,4);
	$res = substr($m,5,3);
	$chain = substr($m,9,1);
		if (($ar2[0] == $irold) && ($ar2[1] eq $res) && ($ar2[2] eq $chain))
		{
		$ar2[0] = $irnew;
		printf OUT2 "%3d%1s%3s %1s\n",$ar2[0],'-',$ar2[1],$ar2[2]; 
		printf RAS2 "%14s %3d\n","select :$chain2 and ",$ar2[0];
#		$rasstr2 .= sprintf("%8s%3d%2s"," :$chain2 and ",$ar2[0],"\, "); 
		}
	}
}

$l1 = length($rasstr1);
$l2 = length($rasstr2);

$rstr1 = substr($rasstr1,0,$l1-2);
$rstr2 = substr($rasstr2,0,$l2-2);

#print RAS $rstr1,"\n";

#print RAS $rstr2,"\n";



