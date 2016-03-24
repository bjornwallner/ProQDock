#!/usr/bin/perl

$inp1f = $ARGV[0];	# inner contour
chomp $inp1f;
$inp2f = $ARGV[1];	# outer contour
chomp $inp2f;
$sqgrd = $ARGV[2];	# Pgrid values
chomp $sqgrd;
$realinp = $ARGV[3];	# realtime data
chomp $realinp;
$conttag = $ARGV[4];	# provission for contour drawing 
chomp $conttag;

if ($conttag =~ m/^\s*$/)
{
$conttag = 0;
}

#==============================================
open (INP3,"<$sqgrd");
@datg = <INP3>;
close INP3;

foreach $k (@datg)
{
chomp $k;
$el = substr($k,0,8);
$eu = substr($k,10,8);
$sl = substr($k,20,8);
$su = substr($k,30,8);
$Pgrid = substr($k,50,5);
	if ($Pgrid >= 0.005)
	{
	$ctag = '0.33 0.33 1 setrgbcolor';
	}
	elsif ($Pgrid < 0.005 && $Pgrid >= 0.002)
	{
	$ctag = '0.67 0.67 1 setrgbcolor';
	}
	elsif ($Pgrid < 0.002)
	{
	$ctag = '0 1 1 setrgbcolor';
	}

print "newpath\n";
print "$ctag\n";
print "$sl $el moveto\n";
print "$su $el lineto\n";
print "$su $eu lineto\n";
print "$sl $eu lineto\n";
print "closepath\n";
print "gsave\n";
print "fill\n";
print "grestore\n";
print "stroke\n\n";

}

#=========== FILL UP LEFT BOTTOM CORNER =========

print "\n\nnewpath
0 1 1 setrgbcolor
100 100 moveto
110 100 lineto
110 110 lineto
100 110 lineto
closepath
gsave
fill
grestore
stroke\n";
#==============================================

@holdn = split(/\./,$realinp);

#open (OUT,">ch");
#print OUT "$realinp  $holdn[0]  $holdn[1]\n";

$pdb = $holdn[0];

$bur = $holdn[1];

if ($bur eq 'rcb')
{
$pbur = '0.0 <= residue burial <= 0.05';
}
elsif ($bur eq 'rpb')
{
$pbur = '0.05 < residue burial <= 0.15';
}
elsif ($bur eq 'rpe')
{
$pbur = '0.15 < residue burial <= 0.30';
}

$pre = "%!PS
0 0 0 setrgbcolor
%0.7 0.3 0 setrgbcolor
100 100 moveto 500 100 lineto
gsave
grestore
500 500 lineto
stroke
%===============================
%0.7 0.3 0 setrgbcolor
100 100 moveto 100 500 lineto
gsave
grestore
500 500 lineto
stroke
%===============================
510  90 moveto 510  90 lineto
/Helvetica 20 selectfont
(1,-1) show
%===============================
%0.7 0.3 0 setrgbcolor
80 510 moveto 80 510 lineto
/Helvetica 20 selectfont
(-1,1) show
%===============================
%0.7 0.3 0 setrgbcolor
80 80 moveto 80 80 lineto
/Helvetica 20 selectfont
(-1,-1) show
%===============================
%0.7 0.3 0 setrgbcolor
500 500 moveto 500 500 lineto
/Helvetica 20 selectfont
(1,1) show
%===============================
%0.7 0.3 0 setrgbcolor
510 295 moveto 510 295 lineto
/Helvetica 20 selectfont
(1,0) show
%===============================
%0.7 0.3 0 setrgbcolor
 60 295 moveto  60 295 lineto
/Helvetica 20 selectfont
(-1,0) show
%===============================
%0.7 0.3 0 setrgbcolor
290  80 moveto 290  80 lineto
/Helvetica 20 selectfont
(0,-1) show
%===============================
%0.7 0.3 0 setrgbcolor
290 510 moveto 290 510 lineto
/Helvetica 20 selectfont
(0,1) show
%===============================
%===============================
%===============================
%===============================
";

$pro1 = "%===============================
%===============================
%===============================
%===============================
0 0 0 setrgbcolor
100 300 moveto 500 300 lineto
gsave
grestore
100 300 lineto
stroke
%===============================
0 0 0 setrgbcolor
300 100 moveto 300 500 lineto
gsave
grestore
300 100 lineto
stroke
%===============================
0 0 0 setrgbcolor
300 300 moveto 300 300 lineto
/Helvetica 20 selectfont
(0,0) show
%===============================
%/Helvetica 10 selectfont
%(the effect of showing this string is isolated) show
% CURRENT POINT IS AT END OF STRING (306+? 396)
%grestore
% CURRENT POINT IS AT BEGIN OF STRING (306 396)
%152 492 lineto
%stroke
%===============================
0 0 1 setrgbcolor
200 580 moveto 200 580 lineto
/Helvetica 25 selectfont
(Complementarity Plot) show
285 560 moveto 285 560 lineto
/Helvetica 15 selectfont
($pdb) show
225 540 moveto 225 540 lineto
/Helvetica 15 selectfont
($pbur) show
%30.0 Center\n";


$pro2 = "%===============================
0 0 1 setrgbcolor


%===============================
/Times-Roman findfont 20 scalefont setfont
newpath
300 50 moveto
(S) show
/Times-Roman findfont 10 scalefont setfont
312 48 moveto
(m) show
stroke
/Times-Roman findfont 10 scalefont setfont
312 60 moveto
(sc) show
stroke

%===============================


/Times-Bold findfont 30 scalefont setfont
/ps
{50 300 moveto () show } def
gsave

/Times-Bold findfont 10 scalefont setfont 
/ps2
{50 313 moveto () show } def
gsave

/Times-Bold findfont 10 scalefont setfont   
/ps3
{42 313 moveto () show } def
gsave

%===============================
    
ps
50 300 translate
90 rotate
1 1 scale
newpath
0 0 moveto
/Times-Roman findfont 20 scalefont setfont
(E) show
ps

grestore
gsave		 

%===============================
    
ps2
50 313 translate
90 rotate
1 1 scale
newpath
0 0 moveto
/Times-Roman findfont 10 scalefont setfont
(m) show
ps2

grestore
gsave

%===============================

ps3
42 313 translate
90 rotate
1 1 scale
newpath
0 0 moveto
/Times-Roman findfont 10 scalefont setfont
(sc) show
ps3

grestore
gsave

%===============================
";




open (INP1,"<$inp1f");
@dat1 = <INP1>;
open (INP2,"<$inp2f");
@dat2 = <INP2>;
open (INP3,"<$realinp");
@datr = <INP3>;

$l1 = @dat1;
$l2 = @dat2;
$l3 = @datr;
foreach (@dat1){chomp $_;}
foreach (@dat2){chomp $_;}
foreach (@datr){chomp $_;}

#print "0.7 0.3 0.0 setrgbcolor\n";
print $pre,"\n";

#=====================================================
#  PROVISSION FOR DRAWING CONTOUR LINES 
#=====================================================

if ($conttag == 1)
{
	for $i (1..$l1-2)
	{
	print "1 0 0 setrgbcolor\n";
	print "$dat1[$i-1]  moveto $dat1[$i] lineto\n";
	print "gsave\n";
	print "grestore\n";
	print "$dat1[$i+1] lineto\n";
	print "stroke\n";
	}

	for $i (1..$l2-2)
	{
	print "1 0 1 setrgbcolor\n";
	print "$dat2[$i-1]  moveto $dat2[$i] lineto\n";
	print "gsave\n";
	print "grestore\n";
	print "$dat2[$i+1] lineto\n";
	print "stroke\n";
	}
}

for $i (0..$l3-1)
{
$res = substr($datr[$i],0,3);
$point = substr($datr[$i],4,19);
print "0 0 0 setrgbcolor\n";
print "newpath $point 2 0 360 arc fill closepath stroke\n";
print "$point moveto\n";
print "gsave\n";
#print"/Helvetica 5 selectfont\n";
#print "($res) show\n";
#print "grestore\n";
}

#print "0 0 1 setrgbcolor\n";
print $pro1,"\n";

#----------------------------
# Draw horizontal grid lines
#----------------------------

print "[3 3] 0 setdash\n";

for ($i=100;$i<=500;$i+=10)
{
print "1 0 0.5 setrgbcolor\n";
print "100 $i moveto 500 $i lineto\n";
print "gsave\n";
print "stroke\n";
}

#----------------------------
# Draw Vertical grid lines
#----------------------------

for ($i=100;$i<=500;$i+=10)
{
print "1 0 0.5 setrgbcolor\n";
print "$i 100 moveto $i 500 lineto\n";
print "gsave\n";
print "stroke\n";
}

print $pro2,"\n";



print "showpage % END OF PROGRAM\n";


