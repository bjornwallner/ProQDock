#!/bin/csh

set ccp = $argv[1]

	sed -e '1,$s/HIE/HIS/' $ccp > temp1
	sed -e '1,$s/HID/HIS/' temp1 > temp2
	sed -e '1,$s/HIP/HIS/' temp2 > temp1
#############################################################################
#  DO CHANGE HIP (Positively charged Histidine) to HIS
#############################################################################
	sed -e '1,$s/CYX/CYS/' temp1 > temp2

	sed -e '1,$s/GNN/GLY/'  temp2 > temp1
	sed -e '1,$s/ANN/ALS/'  temp1 > temp2
	sed -e '1,$s/VNN/VAL/'  temp2 > temp1
	sed -e '1,$s/LNN/LEU/'  temp1 > temp2
	sed -e '1,$s/INN/ILE/'  temp2 > temp1
	sed -e '1,$s/FNN/PHE/'  temp1 > temp2
	sed -e '1,$s/YNN/TYR/'  temp2 > temp1
	sed -e '1,$s/WNN/TRP/'  temp1 > temp2
	sed -e '1,$s/MNN/MET/'  temp2 > temp1
	sed -e '1,$s/CNN/CYS/'  temp1 > temp2
	sed -e '1,$s/SNN/SER/'  temp2 > temp1
	sed -e '1,$s/TNN/THR/'  temp1 > temp2
	sed -e '1,$s/QNN/GLN/'  temp2 > temp1
	sed -e '1,$s/NNN/ASN/'  temp1 > temp2
	sed -e '1,$s/ENN/GLU/'  temp2 > temp1
	sed -e '1,$s/DNN/ASP/'  temp1 > temp2
	sed -e '1,$s/PNN/PRO/'  temp2 > temp1
	sed -e '1,$s/HNN/HIS/'  temp1 > temp2
	sed -e '1,$s/RNN/ARG/'  temp2 > temp1
	sed -e '1,$s/KNN/LYS/'  temp1 > temp2

	sed -e '1,$s/GCC/GLY/'  temp2 > temp1
	sed -e '1,$s/ACC/ALS/'  temp1 > temp2
	sed -e '1,$s/VCC/VAL/'  temp2 > temp1
	sed -e '1,$s/LCC/LEU/'  temp1 > temp2
	sed -e '1,$s/ICC/ILE/'  temp2 > temp1
	sed -e '1,$s/FCC/PHE/'  temp1 > temp2
	sed -e '1,$s/YCC/TYR/'  temp2 > temp1
	sed -e '1,$s/WCC/TRP/'  temp1 > temp2
	sed -e '1,$s/MCC/MET/'  temp2 > temp1
	sed -e '1,$s/CCC/CYS/'  temp1 > temp2
	sed -e '1,$s/SCC/SER/'  temp2 > temp1
	sed -e '1,$s/TCC/THR/'  temp1 > temp2
	sed -e '1,$s/QCC/GLN/'  temp2 > temp1
	sed -e '1,$s/CCN/ASN/'  temp1 > temp2
	sed -e '1,$s/ECC/GLU/'  temp2 > temp1
	sed -e '1,$s/DCC/ASP/'  temp1 > temp2
	sed -e '1,$s/PCC/PRO/'  temp2 > temp1
	sed -e '1,$s/HCC/HIS/'  temp1 > temp2
	sed -e '1,$s/RCC/ARG/'  temp2 > temp1
	sed -e '1,$s/ALS/ALA/'  temp1 > temp2
	sed -e '1,$s/ALS/ALA/'  temp2 > temp1
	sed -e '1,$s/NCC/ASN/'  temp1 > temp2
        sed -e '1,$s/  nan/0.000/g' temp2 > temp1		# nan -> 0.00
        sed -e '1,$s/  NaN/0.000/g' temp1 > temp2		# nan -> 0.00
        sed -e '1,$s/  NAN/0.000/g' temp2 > temp1		# nan -> 0.00
	sed -e '1,$s/KCC/LYS/'  temp1 > temp2
#=================== N terminal CYS =============================
	sed -e '1,$s/CSN/CYS/' temp2 > temp1
	sed -e '1,$s/CXN/CYS/' temp1 > temp2
#=================== N terminal HIS =============================
	sed -e '1,$s/HEN/HIS/' temp2 > temp1
	sed -e '1,$s/HDN/HIS/' temp1 > temp2
	sed -e '1,$s/HPN/HIS/' temp2 > temp1
#=================== C terminal CYS =============================
	sed -e '1,$s/CSC/CYS/' temp1 > temp2
	sed -e '1,$s/CXC/CYS/' temp2 > temp1
#=================== C terminal HIS =============================
	sed -e '1,$s/HEC/HIS/' temp1 > temp2
	sed -e '1,$s/HDC/HIS/' temp2 > temp1
	sed -e '1,$s/HPC/HIS/' temp1 > $ccp
