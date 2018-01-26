/*////////////////////////////////////////////////////////////////
Permission to use, copy, modify, and distribute this program for
any purpose, with or without fee, is hereby granted, provided that
the notices on the head, the reference information, and this
copyright notice appear in all copies or substantial portions of
the Software. It is provided "as is" without express or implied
warranty.
*////////////////////////////////////////////////////////////////
#include "CommonPara.h"
#include "ParsePDB.h"
#include "ProteinSurface.h"
#include <time.h>
int main(int argc, char** argv)
{
	ParsePDB pp;
	ProteinSurface pps;
	int inum[4];
	double fnum[2];
	char inname[200];
	char tpname[200];
	char filename[200];
	bool flagopt[7];
	bool bcolor;
	int i;
	clock_t remcstart, remcfinish;
	if(argc<3)
	{
		printf("usage: EDTSurf -i pdbname ...\n");
		printf("-o outname\n");
		printf("-t 1-MC 2-VCMC\n");
		printf("-s 1-VWS 2-SAS 3-MS 4-SES\n");
		printf("-c 1-pure 2-atom 3-chain\n");
		printf("-p probe radius [0,2.0]\n");
		printf("-f scale factor (0,20.0]\n");
		printf("-h 1-in and out 2-out 3-in\n");
		printf("output1: outname.ply\n");
		printf("output2: outname.asa\n");
		printf("output3: outname-cav.pdb\n");
		return 0;
	}
	for(i=0;i<7;i++)
	{
		flagopt[i]=false;
	}
	//default value
	inum[0]=2;//1-MC 2-VCMC
	inum[1]=3;//1-VWS 2-SAS 3-MS 4-SES
	inum[2]=2;//1-pure 2-atom 3-chain
	inum[3]=1;//1-in and out 2-out 3-in
	fnum[0]=1.4;//probe radius
	fnum[1]=4.00;//scale factor
	strcpy(tpname,"out");
	i=1;
	//set value
	while(i<argc)
	{
		if(strcmp(argv[i],"-i")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				strcpy(inname,argv[i+1]);
				i+=2;
			}
			else 
			{
				printf("please set value after -i\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-o")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[0]=true;
				strcpy(tpname,argv[i+1]);
				i+=2;
			}
			else 
			{
				printf("please set value after -o\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-t")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[1]=true;
				int tval=atoi(argv[i+1]);
				if(tval==1) inum[0]=1;
				else if(tval==2) inum[0]=2;
				else
				{
					printf("omit wrong value after -t\n");
				}
				i+=2;
			}
			else 
			{
				printf("please set value after -t\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-s")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[2]=true;
				int tval=atoi(argv[i+1]);
				if(tval==1) inum[1]=1;
				else if(tval==2) inum[1]=2;
				else if(tval==3) inum[1]=3;
				else if(tval==0) inum[1]=0;
				else
				{
					printf("omit wrong value after -s\n");
				}
				i+=2;
			}
			else 
			{
				printf("please set value after -s\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-c")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[3]=true;
				int tval=atoi(argv[i+1]);
				if(tval==1) inum[2]=1;
				else if(tval==2) inum[2]=2;
				else if(tval==3) inum[2]=3;
				else
				{
					printf("omit wrong value after -c\n");
				}
				i+=2;
			}
			else 
			{
				printf("please set value after -c\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-h")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[7]=true;
				int tval=atoi(argv[i+1]);
				if(tval==1) inum[3]=1;
				else if(tval==2) inum[3]=2;
				else if(tval==3) inum[3]=3;
				else
				{
					printf("omit wrong value after -h\n");
				}
				i+=2;
			}
			else 
			{
				printf("please set value after -c\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-p")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[4]=true;
				double tval=atof(argv[i+1]);
				if(tval>0 && tval<2.0) fnum[0]=tval;
				else
				{
					printf("omit wrong value after -p\n");
				}
				i+=2;
			}
			else 
			{
				printf("please set value after -p\n");
				i+=1;
			}
		}
		else if(strcmp(argv[i],"-f")==0)
		{
			if(i+1<argc && !(argv[i+1][0]=='-' && (argv[i+1][1]=='o' || argv[i+1][1]=='t' || argv[i+1][1]=='s' || 
				argv[i+1][1]=='c' || argv[i+1][1]=='p' || argv[i+1][1]=='f' || argv[i+1][1]=='h')))
			{
				flagopt[5]=true;
				double tval=atof(argv[i+1]);
				if(tval>0.5 && tval<=20.0) fnum[1]=tval;
				else
				{
					printf("omit wrong value after -f\n");
					fnum[1]=0.5;
				}
				i+=2;
			}
			else 
			{
				printf("please set value after -f\n");
				i+=1;
			}
		}
		else 
		{
			printf("omit wrong option\n");
			i+=1;
		}
	}
	if(!flagopt[0])
	{
		int len,lpre;
		len=strlen(inname);
		lpre=len-1;
		while(lpre>=0 && inname[lpre]!='.' && inname[lpre]!='/' && inname[lpre]!='\\')
			lpre--;
		if(inname[lpre]=='.')
			lpre--;
		if(lpre>=0)
		{
			strncpy(tpname,inname,lpre+1);
			tpname[lpre+1]='\0';
		}
	}
	printf("The specification is: input: %s, ",inname);
	if(inum[0]==1)
		printf("triangulation: MC, ");
	else printf("triangulation: VCMC, ");
	if(inum[1]==1)
		printf("surface: VWS, ");
	else if(inum[1]==2 || inum[1]==0)
		printf("surface: SAS, ");
	else printf("surface: MS, ");
	if(inum[2]==1)
		printf("color: pure, ");
	else if(inum[2]==2)
		printf("color: atom, ");
	else printf("color: chain, ");
	if(inum[3]==1)
		printf("surface: outer+inner, ");
	else if(inum[3]==2)
		printf("surface: outer, ");
	else printf("surface: inner, ");
	printf("radius: %6.3f, ",fnum[0]);
	printf("scale: %6.3f, ",fnum[1]);
	printf("outname: %s\n",tpname);
	printf("Load file...\n");
	bcolor=true;
	pp.loadpdb(inname);
	pp.extractbb(0,-1,1);
//	pp.PCA();
	pps.proberadius=fnum[0];
	pps.fixsf=fnum[1];
	remcstart=clock(); 
	if(inum[1]==1)
	{
		printf("Initialize...\n");
		pps.initpara(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,false,false);
		printf("actual boxlength %3d, box[%3d*%3d*%3d], scale factor %6.3f\n",
			pps.boxlength,pps.plength,pps.pwidth,pps.pheight,pps.scalefactor);
		printf("Build van der Waals solid\n");
		pps.fillvoxels(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,false,pp.proseq,bcolor);
		pps.buildboundary();
		printf("Build triangulated surface\n");
		if(inum[0]==1)
			pps.marchingcubeorigin2(1);
		else if(inum[0]==2)
			pps.marchingcube(1);
	}
	else if(inum[1]==2)
	{
		printf("Initialize...\n");
		pps.initpara(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,false,true);
		printf("actual boxlength %3d, box[%3d*%3d*%3d], scale factor %6.3f\n",
			pps.boxlength,pps.plength,pps.pwidth,pps.pheight,pps.scalefactor);
		printf("Build solvent-accessible solid\n");
		pps.fillvoxels(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,false,pp.proseq,bcolor);
		pps.buildboundary();
		printf("Build triangulated surface\n");
		if(inum[0]==1)
			pps.marchingcubeorigin2(3);
		else if(inum[0]==2)
			pps.marchingcube(3);
	}
	else if(inum[1]==3)
	{
		printf("Initialize...\n");
		pps.initpara(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,true,true);
		printf("actual boxlength %3d, box[%3d*%3d*%3d], scale factor %6.3f\n",
			pps.boxlength,pps.plength,pps.pwidth,pps.pheight,pps.scalefactor);
		printf("Build solvent-accessible solid\n");
		pps.fillvoxels(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,true,pp.proseq,bcolor);
		pps.buildboundary();
		printf("Euclidean Distance Transform\n");
		pps.fastdistancemap(0);
		printf("Build triangulated surface\n");
		if(inum[0]==1)
			pps.marchingcubeorigin2(4);
		else if(inum[0]==2)
			pps.marchingcube(4);
	}
	else if(inum[1]==4)
	{
		printf("Initialize...\n");
		pps.initpara(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,false,true);
		printf("actual boxlength %3d, box[%3d*%3d*%3d], scale factor %6.3f\n",
			pps.boxlength,pps.plength,pps.pwidth,pps.pheight,pps.scalefactor);
		printf("Build solvent-accessible solid\n");
		pps.fillvoxels(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,false,pp.proseq,bcolor);
		pps.buildboundary();
		printf("Euclidean Distance Transform\n");
		pps.fastdistancemap(0);
		printf("Build van der Waals solid\n");
		pps.boundingatom(false);
		pps.fillvoxelswaals(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,false,pp.proseq,bcolor);
		printf("Build triangulated surface\n");
		if(inum[0]==1)
			pps.marchingcubeorigin2(2);
		else if(inum[0]==2)
			pps.marchingcube(2);
	}
	else if(inum[1]==0)
	{
		printf("Initialize...\n");
		pps.initpara(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,true,true);
		printf("actual boxlength %3d, box[%3d*%3d*%3d], scale factor %6.3f\n",
			pps.boxlength,pps.plength,pps.pwidth,pps.pheight,pps.scalefactor);
		printf("Build solvent-accessible solid\n");
		pps.fillvoxels(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,true,pp.proseq,bcolor);
		pps.buildboundary();
		printf("Euclidean Distance Transform\n");
		pps.fastdistancemap(1);
		printf("Calculate Depth\n");
		pps.calcdepth(pp.numbb,pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,pp.bb,true);
	}
	remcfinish=clock(); 
	double duration = (double)(remcfinish - remcstart)/CLOCKS_PER_SEC;
	printf("Total time %.3f seconds\n",duration);
	//depth
	if(inum[1]==0)
	{
		printf("Output atom depth and residue depth\n");
		sprintf(filename,"%s_atom.dep",tpname);
		FILE *file;
		file=fopen(filename,"wt");
		fprintf(file,"index atom# res#   depth\n");
		for(i=pp.promod[0].procha[0].chainseg.init;i<=pp.promod[0].procha[pp.promod[0].nchain].chainseg.term;i++)
		{
			fprintf(file,"%5d %5d %4d %7.3f\n",i-pp.promod[0].procha[0].chainseg.init+1,pp.proseq[i].seqno,pp.proseq[i].resno,pps.depval[i]);
		}
		fclose(file);
		sprintf(filename,"%s_res.dep",tpname);
		file=fopen(filename,"wt");
		fprintf(file,"indx res# A   depth\n");
		for(i=0;i<pp.numbb;i++)
		{
			double tsum=0;
			int tnum=0;
			for(int k=pp.bb[i].istart;k<=pp.bb[i].iend;k++) 
			{
				tsum+=pps.depval[k];
				tnum++;
			}
			tsum/=double(tnum);
			fprintf(file,"%4d %4d %c %7.3f\n",i+1,pp.bb[i].resind,pp.bb[i].resid,tsum);
		}
		fclose(file);
		return 1;
	}
	//additional functions below
	pps.checkEuler();
	pps.computenorm();
	printf("No. vertices %d, No. triangles %d\n",pps.vertnumber,pps.facenumber);	
	pps.calcareavolume();
	printf("Total area %.3f and volume %.3f\n",pps.sarea,pps.svolume);
	printf("Distinguish inner and outer surfaces\n");
	pps.surfaceinterior();
	printf("Calculate cavity number...\n");
	pps.cavitynumbers();
	printf("Cavity number is %d\n",pps.ncav);
	printf("Calculate cavity area and volume...\n");
	pps.cavitiesareavolume();
	printf("Cavity area %.3f and volume %.3f\n",pps.carea,pps.cvolume);
	printf("Calculate inner and outer atoms\n");
	pps.atomsinout(pp.promod[0].procha[0].chainseg.init,pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq);
	if(pps.ncav>0)
	{	
		printf("Output cavity information\n");
		sprintf(filename,"%s-cav.pdb",tpname);
		pps.outputcavityatoms(pp.promod[0].procha[0].chainseg.init,
			pp.promod[0].procha[pp.promod[0].nchain].chainseg.term,pp.proseq,filename);
	}
	pps.laplaciansmooth(1);
	pps.computenorm();
	printf("Output 3D model\n");
	pps.checkinoutpropa();
	sprintf(filename,"%s.ply",tpname);	
	pps.outputply(filename,pp.proseq,inum[2]-1,inum[3]-1);	
	if(inum[1]==2)
	{
		printf("Output residue surface area\n");
		sprintf(filename,"%s.asa",tpname);
		pps.outsas(pp.numbb,pp.bb,pp.proseq,filename);
	}
	return 1;
} 

