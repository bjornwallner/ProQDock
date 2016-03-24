#!/usr/bin/perl

open (RAS,">view-surf.spt");

$file1 = $ARGV[0];		# surfpdb1	
chomp $file1;

$file2 = $ARGV[1];		# surfpdb2
chomp $file2;

open (INP1,"<$file1");
open (INP2,"<$file2");

@dat1 = <INP1>;
@dat2 = <INP2>;

chomp $dat1[0];
chomp $dat2[0];

$chain1 = substr($dat1[0],21,1);
$chain2 = substr($dat2[0],21,1);

#print "$chain1  $chain2\n";

#open (I1,"<a.pdb") || die "a.pdb not found\n";
#open (I2,"<b.pdb") || die "b.pdb not found\n";

#@d1 = <I1>;
#@d2 = <I2>;

#close I1;
#close I2;

#open (OUTI1,">x.pdb");
#open (OUTI2,">y.pdb");

#foreach $a (@d1)
#{
#chomp $a;
#$ll = length($a)-21;
#$a1 = substr($a,0,21).'X'.substr($a,22,$ll);
#print OUTI1 $a1,"\n";
#}

#foreach $a (@d2)
#{
#chomp $a;
#$ll = length($a)-21;
#$a1 = substr($a,0,21).'Y'.substr($a,22,$ll);
#print OUTI2 $a1,"\n";
#}

#close OUTI1;
#close OUTI2;

#`rm -f a.pdb b.pdb`;

#`cat $file1 $file2 x.pdb y.pdb > surfinp.pdb`;
`cat $file1 $file2 > surfinp.pdb`;

print RAS "load surfinp.pdb\n";
print RAS "wireframe off\n";
print RAS "spacefill 10\n";
print RAS "select :$chain1\n";
print RAS "color green\n";
print RAS "select :$chain2\n";
print RAS "color pink\n";
#print RAS "select :X\n";
#print RAS "wireframe 20\n";
#print RAS "color cpk\n";
#print RAS "select :Y\n";
#print RAS "wireframe 20\n";
#print RAS "color cpk\n";

open (SEL1,"<ras1.select") || die "ras1.select not found\n";
open (SEL2,"<ras2.select") || die "ras2.select not found\n";

@select1 = <SEL1>;
@select2 = <SEL2>;

@col = ('white','blue');

for $a (0..scalar(@select1)-1)
{
chomp $select1[$a];
print RAS $select1[$a],"\n";
print RAS"color $col[0]\n"; 
}

for $a (0..scalar(@select2)-1)
{
chomp $select2[$a];
print RAS $select2[$a],"\n";
print RAS "color $col[1]\n"; 
}

