#!/usr/bin/perl

#====================================================================================
#  Check compatibility of the input file
#  Reject the input file upon satisfaction of wither of the following events:
#------------------------------------------------------------------------------
#  (1) If it does not contain two and exactly two polypeptide chains
#  or
#  (2) Presence of a non-natural amino acid (Check for MSE with record ID HETATM)
#  or
#  (3) Absence of any hydrogen atoms
#  or
#  (4) Incompatable format of Hydrogen atoms (i.e., inconsistent with REDUCE version 2)
#  or 
#  (5) If multiple occupancies are found for atoms
#  or
#  (6) If only main-chain (backbone) atoms are provided
#====================================================================================


$inppdb = $ARGV[0];
chomp $inppdb;
open (PDBFILE,"<$inppdb");
@fdat = <PDBFILE>;

print "\n\nINPUT PDB : $inppdb\n\n";

@coords = grep(/^ATOM\s+/ || /^HETATM\s/,@fdat); 
@pcoords = grep(/^ATOM\s/,@fdat);
@hcoords = grep(/^HETATM\s/,@fdat);

foreach $k (@coords)
{
chomp $k;
}

#===================================================================
# Check 1 : If it is an oligomeric protein (Number of polypeptide chains > 1)
#===================================================================

$side_chain_atoms = 0;

@ch=();
foreach $k (@pcoords)
{
    chomp $k;
#    @ch = (@ch,substr($k,21,1));
    push(@ch,substr($k,21,1));
#    next if($side_chain_atoms>0);
    $atom = substr($k,13,3);
#print $k."\n";
    if ($atom ne "N  " && $atom ne "CA " && $atom ne "C  " && $atom ne "O  " && $atom ne "H  " && $atom ne "HA ")
    {
	$side_chain_atoms++;
#	last;
    }
}

print "\nNumber of side-chain atoms in the input pdb: $side_chain_atoms\n";
#exit;
%h = ();

foreach $k (@ch)
{
    $h{$k}++;
}

@chain = keys %h;

$nch = @chain;

print $nch,"\n";

if ($nch == 2)
{
print "\n\nThe input pdb file contains \'TWO\' polypeptide chains ($chain[0] & $chain[1]) as required. \n\n";
}
else
{
print "=======================================================================================================\n";
print "\n\nThe input pdb file does not contain exactly \'TWO\' polypeptide chains and thus not acceptable for calculation \n [Rather it contains $nch chains]\n";
	foreach $c (@chain)
	{
	print $c,"\n";
	}
print "\nProgram will exit\n";
print "It should contain \'TWO and exactly TWO\' polypeptide chains\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

#=====================================================================
# Check 2: Presence of a non-natural amino acid (with record ID ATOM)
#=====================================================================

@aa20 = ('GLY','ALA','VAL','LEU','ILE','PHE','TYR','TRP','SER','THR','CYS','MET','ASP','GLU','ASN','GLN','LYS','ARG','PRO','HIS');

$nnA = 0;
@storennA = ();

foreach $k (@pcoords)
{
chomp $k;
$res = substr($k,17,3);
$f = 0;
	foreach $a (@aa20)
	{
		if ($res eq $a)
		{
		$f = 1;
		}
	}
	if ($f == 0)
	{
	$nnA++;
	@storennA = (@storennA,$k);
	}
}

if ($nnA == 0)
{
print "The input pdb file does not contain any non-natural amino-acids as required. \n\n";
}

if ($nnA >= 1)
{
print "=======================================================================================================\n";
print "The input pdb file contains $nnA non-natural amino-acids and thus not acceptable for calculation\n\n";
print "\nProgram will exit\n";
print "=======================================================================================================\n";
	foreach $d (@storennA)
	{
	print $d,"\n";
	}
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}


@imet = ("NA    NA",
         "MG    MG",
         "AL   ALF",
         " K     K",
         "CA    CA",
         "MN    MN",
         "MN   MN3",
         "FE   FE2",
         "FE    FE",
         "CO    CO",
         "CO   3CO",
         "NI    NI",
         "NI   3NI",
         "CU   CU1",
         "CU    CU",
         "ZN    ZN",
         "AG    AG",
         "CD    CD",
         "PT1  TPT",
         "AU    AU",
         "AU   AU3",
         "HG    HG");

@addhet = ();
@rejhet = ();
$nMH = 0;
$nRH = 0;

@junk = ();
$wat = 0;

foreach $h (@hcoords)
{
chomp $h;
$res = substr($h,17,3);
	if ($res ne "HOH")
	{
	@junk = (@junk,$h);
	}
	else
	{
	$wat++;
	}
}

@hcoords = ();
@hcoords = @junk;

print "\n\n$wat Water coordinates trimmed off\n\n";

foreach $h (@hcoords)
{
$atres = substr($h,12,8);
$f2 = 0;
	foreach $w (@imet)
	{
		if ($atres eq $w)
		{
		$f2 = 1;
		}
	}
	if ($f2 == 1)
	{
	$nMH++;
	@addhet = (@addhet,$h);
	}
	elsif ($f2 == 0)
	{
	$nRH++;
	@rejhet = (@rejhet,$h);
	}
}
if ($nMH == 0)
{
print "\n\nNo Isolated metal ion(s) (from the list, see HELP) found in the input pdb file\n\n";
}
elsif ($nMH >= 1)
{
print "\n\n$nMH Isolated metal ion(s) (from the list, see HELP) found in the input pdb file which will be appropreately reformated and taken into account\n\n";
	foreach $a (@addhet)
	{
	print $a,"\n";
	}
}


if ($nRH >= 1)
{
print "\nThe following list of ($nRH) hetero-atom(s) will not be considered in the calculations\n\n";
	foreach $r (@rejhet)
	{
	print $r,"\n";
	}
}

@newlist = (@pcoords,@addhet);
print "\n\n";

#foreach (@newlist)
#{
#print $_,"\n";
#}

#=================================================================
#  Multiple occupancies  (Only consider heavy (non-hydrogen) atoms
#=================================================================
exit; #Skip check who cares?

@nhatoms = ();

foreach $k (@pcoords)
{
chomp $k;
$ch1 = substr($k,12,1);
$ch2 = substr($k,13,1);
	if ($ch1 eq "H" || $ch2 eq "H")
	{
	# Do not consider Hydrogens
	}
	else
	{
	@nhatoms = (@nhatoms,$k);
	}
}



foreach $k (@nhatoms)
{
chomp $k;
#print substr($k,21,1),"\n";
$atomid = substr($k,12,4).'-'.substr($k,17,3).'-'.int(substr($k,23,3)).'-'.substr($k,21,1);
#print $atomid,"\n";
$num{$atomid}++;
}

@atid = keys %num;
@freq = values %num;

$l = @atid;
@mult = ();

$c = 0;
for $i (0..$l-1)
{
        if ($freq[$i] == 1)
        {
        $c++;
        }
        elsif ($freq[$i] > 1)
        {
	$z++;
	$conc = $atid[$i].'~'.$freq[$i];
        @mult = (@mult,$conc);
        }
}

if ($z >= 1)
{
print "=======================================================================================================\n";
print "The input pdb file contains $z atom(s) with Multiple occupancies\n";
	foreach $i (0..$z-1)
	{
	@hold = split(/~/,$mult[$i]);
        printf "%20s  %3d\n",$hold[0],$hold[1];	
	}
print "The program will exit\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}



#============================================================
#  Absence of any Hydrogen atoms 
#  Hydrogen atom format inconsistent with REDUCE (v.2)
#  Residue number exceeds three digit
#============================================================

$nH = 0;
$niH = 0;
$flagRNeg = 0;
$flagRNc = 0;
$fHc = 0;
@ilfH = ();

foreach $k (@pcoords)
{
$Hatom  = substr($k,13,1);
$Hillf  = substr($k,12,1);
$Hcharmm = substr($k,12,4);
$resno  = int(substr($k,22,4));
$resc = substr($k,22,5);
$resc =~ s/\-//g;
$resc=~s/\s//g;
	if ($Hatom eq "H")
	{
	$nH++;
	}
	if ($Hillf eq "H")
	{
	$niH++;
	@ilfH = (@ilfH,$k);
	}
	if ($resc =~ m/\D/)
	{
	print $k,"\n";
	$flagRNc = 1;
	}
	if ($resno <= 0)
	{
	$flagRNeg = 1;
	}
        if ($Hcharmm eq 'HD11' || $Hcharmm eq 'HD12' || $Hcharmm eq 'HD13' || $Hcharmm eq 'HD21' || $Hcharmm eq 'HD23' || $Hcharmm eq 'HD22' || $Hcharmm eq 'HE21' || $Hcharmm eq 'HE22' || $Hcharmm eq 'HG11' || $Hcharmm eq 'HG12' || $Hcharmm eq 'HG13' || $Hcharmm eq 'HG21' || $Hcharmm eq 'HG22' || $Hcharmm eq 'HG23')
	{
	$fHc = 1;
	}
}

if ($nH == 0)
{
print "=======================================================================================================\n";
print "\nThe input pdb file does not contain any Hydrogen atom \n\n(Use: Reduce v.2: http://kinemage.biochem.duke.edu/downloads/software/reduce/ to geometrically fit Hydrogens)\n\n";
print "\nThe program will exit\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

if ($side_chain_atoms == 0)
{
print "=======================================================================================================\n";
print "\nThe input pdb file contains backbone (main-chain) atoms only\n";
print "\nProgram will exit\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

if ($niH >= 1)
{
print "=======================================================================================================\n";
print "\nThe input pdb file contains $niH Hydrogen(s) inconsistent with REDUCE (v.2) format: http://kinemage.biochem.duke.edu/downloads/software/reduce/\n\n";
	foreach $ih (@ilfH)
	{
	print $ih,"\n";
	}
print "\nThe program will exit\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

if ($fHc == 1)
{
print "=======================================================================================================\n";
print "\nThe input pdb file contains Hydrogen(s) inconsistent with REDUCE (v.2) format\n\n";
print "\nThe program will exit\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

if ($flagRNc == 1)
{
print "=======================================================================================================\n";
print "\nThe input pdb file contains residue sequence numbers involving non-integer characters \n\n";
print "\nThe program will exit\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

if ($flagRNeg == 1)
{
print "=======================================================================================================\n";
print "\nThe input pdb file contains residues with sequence numbers zero or negative \n\n";
print "\nThe program will exit\n\n";
print "=======================================================================================================\n";
open (OUTF,">formch.out");
print OUTF "stop\n";
exit;
}

