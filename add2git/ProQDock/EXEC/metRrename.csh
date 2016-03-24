#!/bin/csh

set pdb = $argv[1]

	sed -e '1,$s/NA    NA/ NA1 SOD/'  $pdb > temp1
	sed -e '1,$s/MG    MG/ MG2 MAG/'  temp1 > temp2
	sed -e '1,$s/AL   ALF/ AL3 ALM/'  temp2 > temp1
	sed -e '1,$s/ K     K/ K_1 POT/'  temp1 > temp2
	sed -e '1,$s/CA    CA/ CA2 CAL/'  temp2 > temp1
	sed -e '1,$s/MN    MN/ MN2 MNG/'  temp1 > temp2
	sed -e '1,$s/MN   MN3/ MN3 MNG/'  temp2 > temp1
	sed -e '1,$s/FE   FE2/ FE2 IRN/'  temp1 > temp2
	sed -e '1,$s/FE    FE/ FE3 IRN/'  temp2 > temp1
	sed -e '1,$s/CO    CO/ CO2 COB/'  temp1 > temp2
	sed -e '1,$s/CO   3CO/ CO3 COB/'  temp2 > temp1
	sed -e '1,$s/NI    NI/ NI2 NIC/'  temp1 > temp2
	sed -e '1,$s/NI   3NI/ NI3 NIC/'  temp2 > temp1
	sed -e '1,$s/CU   CU1/ CU1 COP/'  temp1 > temp2
	sed -e '1,$s/CU    CU/ CU2 COP/'  temp2 > temp1
	sed -e '1,$s/ZN    ZN/ ZN2 ZNC/'  temp1 > temp2
	sed -e '1,$s/AG    AG/ AG1 SLV/'  temp2 > temp1
	sed -e '1,$s/CD    CD/ CD2 CDM/'  temp1 > temp2
	sed -e '1,$s/PT1  TPT/ PT2 PLT/'  temp2 > temp1
	sed -e '1,$s/AU    AU/ AU1 GLD/'  temp1 > temp2
	sed -e '1,$s/AU   AU3/ AU3 GLD/'  temp2 > temp1
	sed -e '1,$s/HG    HG/ HG2 MRC/'  temp1 > $pdb



