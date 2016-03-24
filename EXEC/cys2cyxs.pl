#!/usr/bin/perl

# runs at $tag == 0 (from his2hiped.pl)

# for dsl.out (sankar)

open (INP,"<inp.dsl");
@dat = <INP>;

$l = @dat;
#print $l,"\n";

@cys = ();

for $i (0..$l-1)
{
chomp $dat[$i];
	if (substr($dat[$i],7,3) eq 'CYS')
	{
	@cys = (@cys,substr($dat[$i],2,3));
	}
	if (substr($dat[$i],27,3) eq 'CYS')
	{
	@cys = (@cys,substr($dat[$i],22,3));
	}
}

$l1 = @cys;

#print $l1,"\n";

%unq = ();

foreach $k (@cys)
{
$unq{$k}++;
}

@ucys = sort {$a<=>$b} keys %unq;

foreach $k (@ucys)
{
print $k,"\n";
}

$l2 = @ucys;

#print "\n",$l2,"\n";

#print $file,"\n";

#open (INP,"<$file") || die "$file could not be retrieved from the workspace\n";

#@pdat = <INP>;

@rnn = ();

open (HISF,"<outhisO.pdb");
open (OUTCYS,">outhiscysO.pdb");

@pdat = <HISF>;

$cnt = 0;

foreach $p (@pdat)
{
chomp $p;
$rn = substr($p,23,3);
$rt = substr($p,17,3);
	foreach $c (@ucys)
	{
		if ($rn == $c && $rt eq 'CYS')
		{
		$p =~ s/CYS/CYX/;
		@rnn = (@rnn,$rn);
		}
	}
$str1 = substr($p,0,12);
$str2 = substr($p,13, );
print OUTCYS $str1,' ',$str2,"\n";
$cnt++;
}

#print "\n\nNumber of lines in the outfile : $cnt\n\n";


%h = ();

foreach (@rnn)
{
$h{$_}++;
}

@uc = keys %h;
$luc = @uc;

#print "\n\n$luc CYS-> CYX conversions made \n\n";


