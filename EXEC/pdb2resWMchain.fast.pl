#!/usr/bin/perl

$fn = $ARGV[0] || die "Enter PDB filename as the only input argument\n";     # pdb file
chomp $fn;

open (INP,"<$fn");
@dat = <INP>;
close INP;

$resfile = $fn;
$resfile =~ s/\.pdb/\.res/;

open (RES,">$resfile");

@atoms = grep(/^ATOM\s+/,@dat);
@hetatms = grep(/^HETATM\s+/,@dat);

$of = $fn;
$of =~ s/.pdb/.res/g;
#open (OUT,">$of");

# EXTRACT CHAIN

foreach $k (@atoms){chomp $k;}
foreach $k (@hetatms){chomp $k;}

$chain1 = substr($atoms[0],21,1);
$chain2 = substr($atoms[scalar(@atoms)-1],21,1);

if ($chain1 eq $chain2)
{
    @uchain = ($chain1);
}
elsif ($chain1 ne $chain2)
{
    @uchain = ($chain1,$chain2);
}

$l = @uchain;

#printf "%3d\n", $l;

$c = 0;
%temp=();
foreach $chain (@uchain)
{
 #   print $chain,"\n";
    $c++;
    $dat = 'dat'.$c;
#    print $dat."\n";
    @$dat = ();
    foreach $at (@atoms)
    {
	chomp $at;
	$chn = substr($at,21,1);
	if ($chain eq $chn)
	{
	    $r = substr($at,22,4).'-'.substr($at,17,3).'-'.substr($at,21,1);
	    if(not(defined($temp{$r}))) {
		@$dat = (@$dat,$r);
		$temp{$r}=1;
	    }
	       
	}
    }
}
#print scalar(@dat);
#print $c,"\n";
#exit;
$Nres = 0;

@ures = ();

for $i (1..$c)
{
    $dat = 'dat'.$i;
    $hash = 'hash'.$i;
    $udat = 'udat'.$i;
    $$hash = ();
    foreach $r (@$dat)
    {
	$$hash{$r}++;
#	print "$i $r\n";
    }
#print "\n\n";
    @$udat = sort {$a <=> $b} keys %$hash;
    foreach $u (@$udat)
    {
	printf RES "%9s\n",$u;
	@ures = (@ures,$u);
	$Nres++;
    }
}
#exit;
# ADD METALS

%hm = ();

foreach $m (@hetatms)
{
    chomp $m;
    $r = substr($m,22,4).'-'.substr($m,17,3).'-'.substr($m,21,1);
    $hm{$r}++;
}

@um = sort {$a<=>$b} keys %hm;

foreach $u (@um)
{
    printf RES "%10s\n",$u;
#printf "%9s\n",$u;
    $Nres++;
}

print $Nres,"\n";

exit;
#printf "%10s  %3d\n",$fn,$l;

#========================= CHECK REDUNDANCY =============================

%redun = ();

foreach $k (@ures)
{
    $rn = substr($k,0,4).'-'.substr($k,9,1);
    $redun{$rn}++;          # Check redundant residue identities for same position
}

@urn = keys %redun;
@frq = values %redun;

$rrn = 0;
@rdn = ();

for $n (0..scalar(@frq)-1)
{
    if ($frq[$n] > 1)
    {
        $rrn++;
        @rdn = (@rdn,$urn[$n])
    }
}


if ($rrn > 0)
{
    open (LOGR,">rdn.log");
    print LOGR "redundant residue identities found for same position (in $rrn cases)\n";
    print LOGR "for residue position(s):\n";
    foreach $a (@rdn)
    {
        print LOGR "$a\n";
    }
    print LOGR "\n";
}



