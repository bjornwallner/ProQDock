#!/usr/bin/perl

$file = $ARGV[0];	# pdb
chomp $file;

#$tag = $ARGV[1];	# tag for disulphide correction : 0 for dsl.out, 1 for sg.out
#chomp $tag;

# Set default 
$tag = 0;

#if ($tag =~ m/^\s*$/)
#{
#print "\n\nType either 0 (from sankar : dsl.out) or 1 (from rahul : sg.out) for disulphide bridge correction : (CYS -> CYX conversion)\n Default taken as 0\n\n";
#}

open (INP,"<$file");
@dat = <INP>;

$resf = $file;
$resf =~ s/\.pdb/\.res/g;

open (OUT,">outhisO.pdb");

open (RES,"<$resf") || die ".res file not found\n";

@rdat = <RES>;

foreach $k (@rdat)
{
chomp $k;
	if ($k =~ /HIS/)
	{
#	print $k,"\n";
	@store = ();
	@atom = ();
	@pointer = ();
	$d = 0;
	$e = 0;
	$c = 0;
		foreach $p (@dat)
		{
		chomp $p;
			if (substr($p,17,3) eq 'HIS' && int(substr($p,23,3)) == int(substr($k,0,3)))
			{
			@store = (@store,$p);
			@atom = (@atom,substr($p,13,3));
			@pointer = (@pointer,$c);
			}
		$c++;
		}
		foreach (@atom)
		{
			if ($_ eq 'HD1')
			{
			$d = 1;
			}
			if ($_ eq 'HE2')
			{
			$e = 1;
			}
		}
		if ($d == 1 && $e == 0)
		{
			foreach $r (@store)
			{
			$r =~ s/HIS/HID/;
			}
		}
		if ($d == 0 && $e == 1)
		{
			foreach $r (@store)
			{
			$r =~ s/HIS/HIE/;
			}
		}
		if ($d == 1 && $e == 1)
		{
			foreach $r (@store)
			{
			$r =~ s/HIS/HIP/;
			}
		}
		foreach (@store)
		{
#		print $_,"\n";
		}
		for $n (0..scalar(@pointer)-1)
		{
		$dat[$pointer[$n]] = $store[$n];
		}
	}	
}

foreach $k (@dat)
{
chomp $k;
$str1 = substr($k,0,12);
$str2 = substr($k,13, );
print OUT $str1,' ',$str2,"\n";
}

close OUT;

#if ($tag == 0)
#{
#print "\n\nCalling ./EXEC/cys2cyxs.pl for CYS -> CYX conversion\n\n";
#do './EXEC/cys2cyxs.pl';
#}
