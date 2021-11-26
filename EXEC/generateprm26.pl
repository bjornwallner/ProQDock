#!/usr/bin/perl

# Generate script parameter file given the input and output filenames:
# Other parameters will have to be changed manually if required

open (OUT,">script.prm");

$path = $ARGV[0];	# path 
chomp $path;

$inpdb = $ARGV[1];	# input pdb (molecule)
chomp $inpdb;

$infrc = $ARGV[2];	# input frc (grid/surface)
chomp $infrc;

$outpdb = $ARGV[3];	# output modified pdb (charged assigned)
chomp $outpdb;

$outfrc = $ARGV[4];	# output frc (main outfiles with E.P)
chomp $outfrc;

$gsz = $ARGV[5];	# grid size directly fed from the coordinates (Number of points per side of the cubic lattice = ((largest interatomic distance in the protein) / scale) + 25.0)
chomp $gsz;

$gauss = $ARGV[6];
chomp $gauss;

$gsz = int($gsz);	# FOR DELPHI v.6

print OUT "\n
!=============================================================
! The delphi script (.prm) or parameter file (replaces fort.10)
!=============================================================";
print OUT "
!=======================
! grid points / Angstrom
! default : 1.2
!=======================

scale=1.2";
print OUT "
!==============================================================
! Set any one of the following (gsize / perfil) two parameters
! The other would be set automatically
!==============================================================

!===============================================
! grid size
! number of points per side of the cubic lattice 
! default : 65
!===============================================\n";

print OUT "\ngsize=$gsz\n";
	
print OUT "
!=============================================================
! percentage fill
! A percentage of the object longest linear dimension 
! to the lattice linear dimension
! default : 80
!=============================================================

perfil=90         ! default (optimized)";
print OUT "
!=============================================================
! type of boundary condition imposed on the edge of the lattice
! 1. zero potential
! 2. dipolar
! 3. focussing
! 4. coulambic
!=============================================================

bndcon=2          ! default (dipolar)";
print OUT "
!=============================================================
! vdW radii of atoms (replaces fort.11)
!=============================================================

in(siz,file=\"amber_dummy.siz\")";
print OUT "
!=============================================================
! partial charges (replaces fort.12)
!=============================================================

in(crg,file=\"amber.crg\")";
print OUT "
!=============================================================
! internal dielectric (low, of protein)
!=============================================================

indi=2.0";
print OUT "
!=============================================================
! external dielectric (high, of water)
!=============================================================

exdi=80.0";
print OUT "
!=============================================================
! Prob radius for defining solvent accessible surface area and
! hence setting the dielectric boundary
! alternately set 1.4 & 0 for solvent accesible surface of the
! whole molecule and the candidate residue for
! partially solvated protein
!=============================================================";
print OUT "
prbrad=1.4

!=============================================================
! salt concentration / ionic strength (moles/litre)
! introduction of Multiple dielectric constants
! invokes non-linear Poisson-Boltzmann equation solver
! and thus turns on nonlinear iterative contributions automatically
!=============================================================";
print OUT "
salt=0.00";
print OUT "
!=============================================================
! ionic radius
! solvent ion is considered as a point charge
! which can approach no closer than its ionic radius
! to any atoms van der Waals surface.
!=============================================================

ionrad=0.0";

if ($gauss == 1)
{
	print OUT "
	
!============================================================
! multi dielectric modeled as gaussian 
!============================================================

Gaussian=1
sigma=0.93
srfcut=20

";

}

print OUT "

!=============================================================
! The convergence threshold value based on root mean square change of potential
! rmsc = potential root mean square change threshold,[kT/e]
!=============================================================
rmsc=0.0001

!=============================================================
! The convergence threshold value based on maximum change of potential
! maxc = potential maximum change threshold, [kT/e]
!=============================================================

maxc=0.0001

!============================================================
! Number of Linear iterative steps
! sufficient for systems with zero ionic strength
!============================================================";
print OUT "
linit=1000

!============================================================
! Number of non-linear iterative steps (only applicable for
! nonlinear Poisson-Boltzmann equation)
! necessary only for systems with non-zero ionic strength
!============================================================

nonit=800


!============================================================
! Energy (solvation, Coulamb, Grid)
!============================================================

energy(s,c,g,ion)

!=============================================================
! input coordinate file (pdb) (replaces fort.13)
! radii & partial charges would be assigned to these atoms
! these set of atoms would be used to calculate electrostatic potentials
! should not contain HIS (in staid HIP/HID/HIE)
! H atoms should be consistent with the siz / crg files
! i,e., format : 13x,a3 and not 12x,a4
!=============================================================";
print OUT "
in(pdb,file=\"$inpdb\")";
print OUT "
!=============================================================
! input site coordinate file (frc) in pdb format (replaces fort.15)
! molecular (vdW) surface of a residue of interest
! should not be charged assigned
!=============================================================";
print OUT "
in(frc,file=\"$infrc\")";
print OUT "
!==============================================================
! electrostatic potential on the surface : the main out file
!==============================================================";
print OUT "
out(frc,file=\"$outfrc\")

!==============================================================
! print out the following for each surface grid point specified in the frc file
! atom descriptor, coordinates, charge, site potential
!==============================================================";
print OUT "
site(a,x,y,z,q,p)

!==============================================================
! modified charged assigned pdb file (replaces fort.19)
!==============================================================";
print OUT "
out(modpdb,file=\"$outpdb\")	! output charged assigned atomic coordinate file ";
print OUT "
!==============================================================
! generate phimap used in focussing (replaces fort.14) (binary)
!==============================================================

!out(phi,file=\"phimap.phi\")";
!print OUT "
!==============================================================
! solvent ion concentration output in M/l
!==============================================================

!phicon=t

!============================= END ============================";

close(OUT)


