#!/usr/bin/perl

$upath = $ARGV[0] || die "Enter upath\n";
chomp $upath;
$path = `readlink -f $upath`;
chomp $path;
$Sc = $ARGV[1] || die "Enter Sc\n";
$EC = $ARGV[2] || die "Enter EC\n";
$nbsa = $ARGV[3] || die "Enter nBSA\n";

chomp $Sc;
chomp $EC;
chomp $nbsa;

$cinp = scalar(@ARGV);

if ($cinp < 4)
{
die "Enter path, Sc, EC and nBSA as three consecutive cmds\n";
}

@Sclib = ();
@EClib = ();


	if ($nbsa <= 0.05)
	{
#	print "I am at the first bin\n";
	open (SCLIB,"<$path/CPMlib/pScgnBSA1.lib");
	open (ECLIB,"<$path/CPMlib/pECgnBSA1.lib");
	@Sclib = <SCLIB>;
	@EClib = <ECLIB>;
	close SCLIB;
	close ECLIB;
	}
	elsif ($nbsa > 0.05 && $nbsa <= 0.10)
	{
#	print "I am at the second bin\n";
	open (SCLIB,"<$path/CPMlib/pScgnBSA2.lib");
	open (ECLIB,"<$path/CPMlib/pECgnBSA2.lib");
	@Sclib = <SCLIB>;
	@EClib = <ECLIB>;
	close SCLIB;
	close ECLIB;
	}
	elsif ($nbsa > 0.10)
	{
#	print "I am at the third bin\n";
	open (SCLIB,"<$path/CPMlib/pScgnBSA3.lib");
	open (ECLIB,"<$path/CPMlib/pECgnBSA3.lib");
	@Sclib = <SCLIB>;
	@EClib = <ECLIB>;
	close SCLIB;
	close ECLIB;
	}

$pSc = -1.000;
$pEC = -1.000;

	for $i (0..scalar(@Sclib)-1)
	{
	chomp $Sclib[$i];
	chomp $EClib[$i];
#	print $Sclib[$i],"\n";
#	print $EClib[$i],"\n";
	$Sclb = substr($Sclib[$i],0,6);
	$Scub = substr($Sclib[$i],8,6);
	$pScgnbsa = substr($Sclib[$i],21,7);
	$EClb = substr($EClib[$i],0,6);
	$ECub = substr($EClib[$i],8,6);
	$pECgnbsa = substr($EClib[$i],21,7);
#	print "$Sclb  ::  $Scub  ::  $pScgnbsa\n";
#	print "$EClb  ::  $ECub  ::  $pECgnbsa\n";
		if ($Sc >= $Sclb && $Sc <= $Scub)
		{
		$pSc = $pScgnbsa;
		}
		if ($EC >= $EClb && $EC <= $ECub)
		{
		$pEC = $pECgnbsa;
		}
	}


#print "$pSc\n$pEC\n";

$pScEC = $pSc*$pEC;

if ($pScEC == 0)
{
$logodd = sprintf("%10.3f",-100.000);
}
else
{
$logodd = sprintf("%10.3f",(log($pScEC)/log(10.0)));
}

$CPM = $logodd;

$minCP=-5.505;
$maxCP=-1.399;

        if ($CPM == -100.00)
        {
        $nCPM = sprintf("%10.5f",0.00);
        }
        else
        {
        $nCPM = sprintf("%10.5f",(($CPM - $minCP)/($maxCP - $minCP)));
        }

print $nCPM,"\n";


