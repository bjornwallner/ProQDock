#!/usr/bin/perl
# Takes a pdb file and returns the sequence in 1 letter code

$file = $ARGV[0];
chomp $file;

open (INP,"<$file");

my %table=('ALA', 'A',
	'ARG', 'R',
	'ASN', 'N',
	'ASP', 'D',
	'CYS', 'C',
	'GLN', 'Q',
	'GLU', 'E',
	'GLY', 'G',
	'HIS', 'H',
	'ILE', 'I',
	'LEU', 'L',
	'LYS', 'K',
	'MET', 'M',
	'PHE', 'F',
	'PRO', 'P',
	'SER', 'S',
	'THR', 'T',
	'TRP', 'W',
	'TYR', 'Y',
	'VAL', 'V',
        'ASX', 'B',
        'GLX', 'Z',
        'XXX', 'A',
        'MSE', 'M',
        'FME', 'M',
        'PCA', 'E',
        '5HP', 'E',
        'SAC', 'S',
        'CCS', 'C');

$old_resnum="whatever";
print ">$file.fasta","\n";
while(<INP>)
{
    if(/^ATOM/)
    {
	my $atomno=substr($_, 7, 4);
	my $atomtype=substr($_, 13, 3);
	my $resnum=substr($_,21,5);
	$resnum=~s/\s+//g;
	#print "$resnum $old_resnum $atomtype\n";
	#if($atomtype=~/CA/ && $old_resnum eq $resnum)
	#{
	#    print;
	#}
	if($atomtype=~/CA/) # && $old_resnum ne $resnum)
	{
	    $res=substr($_,17, 3);
	    print $table{$res};
	    $old_resnum=$resnum;
	}

	
    }

    last if(/^ENDMDL/);
}
print "\n";
#print "*";



