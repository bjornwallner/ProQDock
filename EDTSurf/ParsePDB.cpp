/*//////////////////////////////////////////////////////////////// 
Permission to use, copy, modify, and distribute this program for 
any purpose, with or without fee, is hereby granted, provided that
the notices on the head, the reference information, and this
copyright notice appear in all copies or substantial portions of 
the Software. It is provided "as is" without express or implied 
warranty.
*////////////////////////////////////////////////////////////////
// ParsePDB.cpp: implementation of the ParsePDB class.
//
//////////////////////////////////////////////////////////////////////
#include "CommonPara.h"
#include "ParsePDB.h"

int jacobi(int n,double a[],double d[],double v[])
{
    int p,q,i,j;
    double sm,c,s,t,h,g,tau,theta,tresh,*b,*z;
    b=(double *)calloc(n,sizeof(double));
	// if(b==NULL) ...
    z=(double *)calloc(n,sizeof(double));
	// if(z==NULL) ...
	
    for(p=1;p<=n;p++)
	{	
		for(q=1;q<=n;q++)
		{
			if(p==q) 
				v[(p-1)*n+q-1]=1.0;
			else    
				v[(p-1)*n+q-1]=0.0;
		}
	}
	for(p=1;p<=n;p++)
	{
		b[p-1]=d[p-1]=a[(p-1)*n+p-1];
		z[p-1]=0.0;
	}
	for(i=1;i<=50;i++)
	{
		sm=0.0;
		for(p=1;p<=n-1;p++)
		{	
			for(q=p+1;q<=n;q++)
			{
				sm=sm+fabs(a[(p-1)*n+q-1]);
			}
		}
		if(sm==0.0)
		{
			free(b);
			free(z);
			return(1);
		}
		if(i<4) 
			tresh=0.2*sm/(n*n);
		else 
			tresh=0.0;
		for(p=1;p<n;p++)
		{		
			for(q=p+1;q<=n;q++)
			{
				g=100*fabs(a[(p-1)*n+q-1]);
				if(i>4 && (fabs(d[p-1])+g)==fabs(d[p-1]) && (fabs(d[q-1]+g)==fabs(d[q-1])) )
					a[(p-1)*n+q-1]=0.0;
				else
				{
					if(fabs(a[(p-1)*n+q-1])>tresh)
					{
						h=d[q-1]-d[p-1];
						if((fabs(h)+g)==fabs(h))
							t=a[(p-1)*n+q-1]/h;
						else
						{
							theta=0.5*h/a[(p-1)*n+q-1];
							t=1/(fabs(theta)+sqrt(1+theta*theta));
							if(theta<0) 
								t=-t;
						}
						c=1/sqrt(1+t*t);
						s=t*c;
						tau=s/(1+c);
						h=t*a[(p-1)*n+q-1];
						z[p-1]-=h;d[p-1]-=h;
						z[q-1]+=h;d[q-1]+=h;
						a[(p-1)*n+q-1]=0.0;
						for(j=i;j<p;j++)
						{
							g=a[(j-1)*n+p-1];
							h=a[(j-1)*n+q-1];
							a[(j-1)*n+p-1]=g-s*(h+g*tau);
							a[(j-1)*n+q-1]=h+s*(g-h*tau);
						}
						for(j=p+1;j<q;j++)
						{
							g=a[(p-1)*n+j-1];
							h=a[(j-1)*n+q-1];
							a[(p-1)*n+j-1]=g-s*(h+g*tau);
							a[(j-1)*n+q-1]=h+s*(g-h*tau);
						}
						for(j=q+1;j<=n;j++)
						{
							g=a[(p-1)*n+j-1];
							h=a[(q-1)*n+j-1];
							a[(p-1)*n+j-1]=g-s*(h+g*tau);
							a[(q-1)*n+j-1]=h+s*(g-h*tau);
						}
						for(j=1;j<=n;j++)
						{
							g=v[(j-1)*n+p-1];
							h=v[(j-1)*n+q-1];
							v[(j-1)*n+p-1]=g-s*(h+g*tau);
							v[(j-1)*n+q-1]=h+s*(g-h*tau);
						}
					}
				}
			}
		}
		for(p=1;p<=n;p++)
		{
			d[p-1]=b[p-1]=b[p-1]+z[p-1];
			z[p-1]=0;
		}
	}
	free(b);
	free(z);
	return(1);
}
int aminoid(char aminoname)
{
	int i;
	if(aminoname==' ')
	{
		return 23;
	}
	for(i=0;i<26;i++)
	{
		if(aminoname==aad1[i])
		{
			return i;
		}
	}
	return 23;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ParsePDB::ParsePDB()
{
	headnumchain=-1;
	indexchain=0;
	int i;
	for(i=0;i<maxnumchains;i++)
	{	
		aminoinchain[i]=NULL;
	}
	pt.ismtrix=false;
	pt.isorigx=false;
	pt.isscale=false;
	helixl=NULL;
	sheetl=NULL;
	turnl=NULL;
	linkl=NULL;
	numhelix=0;numsheet=0;numturn=0;numlink=0;
	allochelix=30;allocsheet=30;allocturn=30;alloclink=30;
	ssbondl=NULL;sltbrgl=NULL;cispepl=NULL;hydbndl=NULL;sitel=NULL;
	numssbond=0;numsltbrg=0;numcispep=0;numhydbnd=0;numsite=0;
	allocssbond=30;allocsltbrg=30;alloccispep=30;allochydbnd=30;allocsite=30;

	numpromod=0;
	allocpromod=1;
	promod=NULL;
	flagfirstmodel=true;
	flagbeginmodel=true;
	numproseq=0;
	allocproseq=800;
	proseq=NULL;

	numbb=0;
	bb=NULL;


}

ParsePDB::~ParsePDB()
{
	int i;
	for(i=0;i<maxnumchains;i++)
	{
		if(aminoinchain[i]!=NULL)
		{	
			free(aminoinchain[i]);
			aminoinchain[i]=NULL;
		}
	}
	if(helixl!=NULL)
	{
		free(helixl);
		helixl=NULL;
	}
	if(sheetl!=NULL)
	{
		free(sheetl);
		sheetl=NULL;
	}
	if(turnl!=NULL)
	{
		free(turnl);
		turnl=NULL;
	}
	if(linkl!=NULL)
	{
		free(linkl);
		linkl=NULL;
	}
	
	if(ssbondl!=NULL)
	{
		free(ssbondl);
		ssbondl=NULL;
	}
	if(sltbrgl!=NULL)
	{
		free(sltbrgl);
		sltbrgl=NULL;
	}
	if(cispepl!=NULL)
	{
		free(cispepl);
		cispepl=NULL;
	}
	if(hydbndl!=NULL)
	{
		free(hydbndl);
		hydbndl=NULL;
	}
	if(sitel!=NULL)
	{
		free(sitel);
		sitel=NULL;
	}
	if(promod!=NULL)
	{
		free(promod);
		promod=NULL;
	}
	if(proseq!=NULL)
	{
		free(proseq);
		proseq=NULL;
	}
	if(bb!=NULL)
	{
		delete[]bb;
		bb=NULL;
	}

}


int ParsePDB::dealline(char *string)
{
	int i,j;
	char tmpstr[255];
	//SEQRES  28 A  360  ALA THR ALA VAL LYS ILE THR LEU LEU                  2ACH  74
	//SEQRES   1 B   40  SER ALA LEU VAL GLU THR ARG THR ILE VAL ARG PHE ASN  2ACH  75
	if (string[0]=='S' && string[1]=='E' && string[2]=='Q' && string[3]=='R'
		&& string[4]=='E' && string[5]=='S')
	{
		char temnum[5];
		temnum[4]='\0';
		char temid;
		int  tempnum;
		bool isanew;
		temid=string[11];
		if(headnumchain==-1)
		{
			isanew=true;
		}
		else if(headidchain[headnumchain]!=temid)
		{
			isanew=true;
		}
		else
		{
			isanew=false;
		}
		for(i=0;i<4;i++)
		{
			temnum[i]=string[13+i];
		}
		tempnum=atoi(temnum);
		if(headnumchain>=maxnumchains-1)
		{
			printf("Too many chains\n");
			return false;
		}
		else if(isanew)//start of a new chain
		{
			if(headnumchain!=-1)
			{
				if(indexchain!=numinchain[headnumchain])
				{		
					sprintf(tmpstr,"ideal%d,actual%d,ideal%d,chain%d",tempnum,indexchain,
						numinchain[headnumchain],headnumchain);
					printf("Not equal as aclaimed %s\n",tmpstr);
				}
			}
			indexchain=0;
			char temami[3];	
			int aminum;
			aminoinchain[++headnumchain]=new char[tempnum];
			headidchain[headnumchain]=temid;
			numinchain[headnumchain]=tempnum;
			for(i=0;i<13;i++)
			{
				for(j=0;j<3;j++)
				{
					temami[j]=string[19+i*4+j];
				}	
				aminum=aminoid(temami);
				if(aminum>=0 && aminum<24 && indexchain<tempnum)
				{
					aminoinchain[headnumchain][indexchain]=aminum;
					indexchain++;
				}
//				else if(aminum==50 && indexchain<tempnum)
//				{
//					aminoinchain[headnumchain][indexchain]=aminum;
//					indexchain++;
//				}
				else if(temami[0]==' ' && temami[1]==' ' && temami[2]==' '  && indexchain!=tempnum)
				{
					printf("Empty but not end1\n");
				}
				else if(aminum>=0 && aminum<24 && !(temami[0]==' ' && temami[1]==' ' && temami[2]==' ')
					&& indexchain==tempnum)
				{
				//	printf("Too many amino1\n");
				}
			}
			
		}
		else// push
		{
			char temami[3];		
			int aminum;
			for(i=0;i<13;i++)
			{
				for(j=0;j<3;j++)
				{
					temami[j]=string[19+i*4+j];
				}
				aminum=aminoid(temami);
				if(aminum>=0 && aminum<24 && indexchain<tempnum)
				{
					aminoinchain[headnumchain][indexchain]=aminum;
					indexchain++;
				}
//				else if(aminum==50 && indexchain<tempnum)
//				{
//					aminoinchain[headnumchain][indexchain]=aminum;
//					indexchain++;
//				}
				else if(temami[0]==' ' && temami[1]==' ' && temami[2]==' ' && indexchain!=tempnum)
				{
					printf("Empty but not end2\n");
				}
				else if(aminum>=0 && aminum<24 && !(temami[0]==' ' && temami[1]==' ' && temami[2]==' ') 
					&&  indexchain==tempnum)
				{
				//	printf("Too many amino2\n");
				}
			}
		}	
		return 1;
	}
	else if (string[0]=='O' && string[1]=='R' && string[2]=='I' && string[3]=='G'
		&& string[4]=='X' ) 
	{
		pt.isorigx=true;
		char cid[2];
		cid[1]='\0';
		cid[0]=string[5];
		int id=atoi(cid)-1;
		double txyz[3],ttran;
		char temxyz[10],temtran[10];
		for(i=0;i<3;i++)
		{
			for(j=0;j<10;j++)
			{
				temxyz[j]=string[10+10*i+j];
			}
			txyz[i]=atof(temxyz);
			pt.origxn[id][i]=txyz[i];
		}
		for(j=0;j<10;j++)
		{
			temtran[j]=string[45+j];
		}
		ttran=atof(temtran);
		pt.origxn[id][3]=ttran;
		return 2;
	}
	else if (string[0]=='S' && string[1]=='C' && string[2]=='A' && string[3]=='L'
		&& string[4]=='E' ) 
	{
		pt.isscale=true;
		char cid[2];
		cid[1]='\0';
		cid[0]=string[5];
		int id=atoi(cid)-1;
		double txyz[3],ttran;
		char temxyz[10],temtran[10];
		for(i=0;i<3;i++)
		{
			for(j=0;j<10;j++)
			{
				temxyz[j]=string[10+10*i+j];
			}
			txyz[i]=atof(temxyz);
			pt.scalen[id][i]=txyz[i];
		}
		for(j=0;j<10;j++)
		{
			temtran[j]=string[45+j];
		}
		ttran=atof(temtran);
		pt.scalen[id][3]=ttran;
		return 3;
	}
	else if (string[0]=='M' && string[1]=='T' && string[2]=='R' && string[3]=='I'
		&& string[4]=='X' ) 
	{
		pt.ismtrix=true;
		char cid[2];
		cid[1]='\0';
		cid[0]=string[5];
		int id=atoi(cid)-1;
		double txyz[3],ttran;
		char temxyz[10],temtran[10];
		for(i=0;i<3;i++)
		{
			for(j=0;j<10;j++)
			{
				temxyz[j]=string[10+10*i+j];
			}
			txyz[i]=atof(temxyz);
			pt.mtrixn[id][i]=txyz[i];
		}
		for(j=0;j<10;j++)
		{
			temtran[j]=string[45+j];
		}
		ttran=atof(temtran);
		pt.mtrixn[id][3]=ttran;
		return 4;
	}
//	HELIX    1   1 ILE A  146  ALA A  148  5                                   3  
//	HELIX    1 A1  GLY     15  GLN     25  1                                1CRP  86
	else if (string[0]=='H' && string[1]=='E' && string[2]=='L' && string[3]=='I'
		&& string[4]=='X' ) 
	{
		if(numhelix==0)
		{
			helixl=new helixlink[allochelix];
		}
		else if(numhelix==allochelix-1)
		{
			allochelix*=2;
			helixl = (helixlink*)realloc(helixl, allochelix*sizeof(helixlink)); 		
		}
		char temami[3],temchainid,temamiindex[5],temclass[3];
		temamiindex[4]='\0';
		temclass[2]='\0';
		int aminum,amiindex,helclass;
		//start
		for(i=0;i<3;i++)
		{
			temami[i]=string[15+i];
		}
		temchainid=string[19];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[21+i];
		}
		amiindex=atoi(temamiindex);
		helixl[numhelix].initchainid=temchainid;
		helixl[numhelix].initid=aminum;
		helixl[numhelix].initindex=amiindex;
		//end
		for(i=0;i<3;i++)
		{
			temami[i]=string[27+i];
		}
		temchainid=string[31];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[33+i];
		}
		amiindex=atoi(temamiindex);
		helixl[numhelix].endchainid=temchainid;
		helixl[numhelix].endid=aminum;
		helixl[numhelix].endindex=amiindex;
		for(i=0;i<2;i++)
		{
			temclass[i]=string[38+i];
		}
		helclass=atoi(temclass);
		int temleng;
		char temlength[6];
		temlength[5]='\0';
		for(i=0;i<5;i++)
		{
			temlength[i]=string[71+i];
		}
		temleng=atoi(temlength);
		helixl[numhelix].helixlength=temleng;
		helixl[numhelix].helixtype=helclass;
		numhelix++;
		return 5;
	}
//	SHEET    1 S1  5 ASP    38  ILE    46  0                                1CRP  91
//	SHEET    2 S1  5 GLU    49  ASP    57 -1  O  LEU    53   N  LYS    42   1CRP  92
//	SHEET    1   B 2 PHE A 215  THR A 218  0                                        
//	SHEET    2   B 2 THR A 221  PHE A 223 -1  N  PHE A 223   O  PHE A 215           		
	else if (string[0]=='S' && string[1]=='H' && string[2]=='E' && string[3]=='E'
		&& string[4]=='T' ) 
	{
		if(numsheet==0)
		{
			sheetl=new sheetlink[allocsheet];
		}
		else if(numsheet==allocsheet-1)
		{
			allocsheet*=2;
			sheetl = (sheetlink*)realloc(sheetl, allocsheet*sizeof(sheetlink)); 		
		}
		char temami[3],temchainid,temamiindex[5],temclass[3];
		temamiindex[4]='\0';
		temclass[2]='\0';
		int aminum,amiindex,sheclass;
		//start
		for(i=0;i<3;i++)
		{
			temami[i]=string[17+i];
		}
		temchainid=string[21];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[22+i];
		}
		amiindex=atoi(temamiindex);
		sheetl[numsheet].initchainid=temchainid;
		sheetl[numsheet].initid=aminum;
		sheetl[numsheet].initindex=amiindex;
		//end
		for(i=0;i<3;i++)
		{
			temami[i]=string[28+i];
		}
		temchainid=string[32];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[33+i];
		}
		amiindex=atoi(temamiindex);
		sheetl[numsheet].endchainid=temchainid;
		sheetl[numsheet].endid=aminum;
		sheetl[numsheet].endindex=amiindex;
		for(i=0;i<2;i++)
		{
			temclass[i]=string[38+i];
		}
		sheclass=atoi(temclass);
		sheetl[numsheet].sheettype=sheclass;

		//curr
		for(i=0;i<4;i++)
		{
			sheetl[numsheet].curratom[i]=string[41+i];
		}
		for(i=0;i<3;i++)
		{
			temami[i]=string[45+i];
		}
		temchainid=string[49];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[50+i];
		}
		amiindex=atoi(temamiindex);
		sheetl[numsheet].currchainid=temchainid;
		sheetl[numsheet].currid=aminum;
		sheetl[numsheet].currindex=amiindex;
		//prev
		for(i=0;i<4;i++)
		{
			sheetl[numsheet].prevatom[i]=string[56+i];
		}
		for(i=0;i<3;i++)
		{
			temami[i]=string[60+i];
		}
		temchainid=string[64];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[65+i];
		}
		amiindex=atoi(temamiindex);
		sheetl[numsheet].prevchainid=temchainid;
		sheetl[numsheet].previd=aminum;
		sheetl[numsheet].previndex=amiindex;
		numsheet++;
		return 6;
	}
//	TURN     1 T1A SER A  11  ASP A  14     TYPE III                        8ATC 377
//	TURN     2 T2A ASP A 129  ASN A 132     TYPE I                          8ATC 378
	else if (string[0]=='T' && string[1]=='U' && string[2]=='R' && string[3]=='N'
		&& string[4]==' ' ) 
	{
		if(numturn==0)
		{
			turnl=new turnlink[allocturn];
		}
		else if(numturn==allocturn-1)
		{
			allocturn*=2;
			turnl = (turnlink*)realloc(turnl, allocturn*sizeof(turnlink)); 		
		}
		char temami[3],temchainid,temamiindex[5];
		temamiindex[4]='\0';
		int aminum,amiindex;
		//start
		for(i=0;i<3;i++)
		{
			temami[i]=string[15+i];
		}
		temchainid=string[19];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[20+i];
		}
		amiindex=atoi(temamiindex);
		turnl[numturn].initchainid=temchainid;
		turnl[numturn].initid=aminum;
		turnl[numturn].initindex=amiindex;
		//end
		for(i=0;i<3;i++)
		{
			temami[i]=string[26+i];
		}
		temchainid=string[30];
		aminum=aminoid(temami);
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[31+i];
		}
		amiindex=atoi(temamiindex);
		turnl[numturn].endchainid=temchainid;
		turnl[numturn].endid=aminum;
		turnl[numturn].endindex=amiindex;
		numturn++;
		return 7;
	}
//  LINK         C   ACE C 100                 N   PTR C 101  
	else if (string[0]=='L' && string[1]=='I' && string[2]=='N' && string[3]=='K'
		&& string[4]==' ' ) 
	{
		if(numlink==0)
		{
			linkl=new linklink[alloclink];
		}
		else if(numlink==alloclink-1)
		{
			alloclink*=2;
			linkl = (linklink*)realloc(linkl, alloclink*sizeof(linklink)); 		
		}
		char temchainid,temamiindex[5];
		temamiindex[4]='\0';
		int amiindex;
		//start
		for(i=0;i<4;i++)
		{
			linkl[numlink].initatom[i]=string[12+i];
		}
		for(i=0;i<3;i++)
		{
			linkl[numlink].initid[i]=string[17+i];
		}
		temchainid=string[21];
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[22+i];
		}
		amiindex=atoi(temamiindex);
		linkl[numlink].initchainid=temchainid;
		linkl[numlink].initindex=amiindex;
		//end
		for(i=0;i<4;i++)
		{
			linkl[numlink].endatom[i]=string[42+i];
		}
		for(i=0;i<3;i++)
		{
			linkl[numlink].endid[i]=string[47+i];
		}
		temchainid=string[51];
		for(i=0;i<4;i++)
		{
			temamiindex[i]=string[53+i];
		}
		amiindex=atoi(temamiindex);
		linkl[numlink].endchainid=temchainid;
		linkl[numlink].endindex=amiindex;
		numlink++;
		return 8;
	}
//	SSBOND   1 CYS E   48    CYS E   51                          2555  
//	SSBOND   1 CYS G  119    CYS G  205  
	else if (string[0]=='S' && string[1]=='S' && string[2]=='B' && string[3]=='O'
		&& string[4]=='N' && string[5]=='D') 
	{
		if(numssbond==0)
		{
			ssbondl=new ssbondlink[allocssbond];
		}
		else if(numssbond==allocssbond-1)
		{
			allocssbond*=2;
			ssbondl = (ssbondlink*)realloc(ssbondl, allocssbond*sizeof(ssbondlink)); 		
		}
		//start
		ssbondl[numssbond].initchainid=string[15];
		char temnumc[5];
		temnumc[4]='\0';
		int temnumi;
		for(i=0;i<4;i++)
		{
			temnumc[i]=string[17+i];
		}
		temnumi=atoi(temnumc);
		ssbondl[numssbond].initindex=temnumi;
		//end
		ssbondl[numssbond].endchainid=string[29];
		for(i=0;i<4;i++)
		{
			temnumc[i]=string[31+i];
		}
		temnumi=atoi(temnumc);
		ssbondl[numssbond].endindex=temnumi;
		numssbond++;
		return 9;
	}
//	HYDBND       NH2 ARG    111                 OD1 ASP    149   1555		
	else if (string[0]=='H' && string[1]=='Y' && string[2]=='D' && string[3]=='B'
		&& string[4]=='N' && string[5]=='D') 
	{
		if(numhydbnd==0)
		{
			hydbndl=new hydbndlink[allochydbnd];
		}
		else if(numhydbnd==allochydbnd-1)
		{
			allochydbnd*=2;
			hydbndl = (hydbndlink*)realloc(hydbndl, allochydbnd*sizeof(hydbndlink)); 		
		}
		//start
		for(i=0;i<4;i++)
		{
			hydbndl[numhydbnd].initatom[i]=string[12+i];
		}
		hydbndl[numhydbnd].initchainid=string[21];
		char temami[3],temseq[5];
		temseq[4]='\0';
		int temnum,temseqi;
		for(i=0;i<3;i++)
		{
			temami[i]=string[17+i];
		}
		temnum=aminoid(temami);
		hydbndl[numhydbnd].initid=temnum;
		for(i=0;i<4;i++)
		{
			temseq[i]=string[22+i];
		}
		temseqi=atoi(temseq);
		hydbndl[numhydbnd].initindex=temseqi;
		//end
		for(i=0;i<4;i++)
		{
			hydbndl[numhydbnd].endatom[i]=string[43+i];
		}
		hydbndl[numhydbnd].endchainid=string[52];
		for(i=0;i<3;i++)
		{
			temami[i]=string[48+i];
		}
		temnum=aminoid(temami);
		hydbndl[numhydbnd].endid=temnum;
		for(i=0;i<4;i++)
		{
			temseq[i]=string[53+i];
		}
		temseqi=atoi(temseq);
		hydbndl[numhydbnd].endindex=temseqi;
		
		numhydbnd++;
		return 10;
	}
//	SLTBRG       O   GLU    10                 NZ  LYS  115             3654		
	else if (string[0]=='S' && string[1]=='L' && string[2]=='T' && string[3]=='B'
		&& string[4]=='R' && string[5]=='G') 
	{
		if(numsltbrg==0)
		{
			sltbrgl=new sltbrglink[allocsltbrg];
		}
		else if(numsltbrg==allocsltbrg-1)
		{
			allocsltbrg*=2;
			sltbrgl = (sltbrglink*)realloc(sltbrgl, allocsltbrg*sizeof(sltbrglink)); 		
		}
		//start
		for(i=0;i<4;i++)
		{
			sltbrgl[numsltbrg].initatom[i]=string[12+i];
		}
		sltbrgl[numsltbrg].initchainid=string[21];
		char temami[3],temseq[5];
		temseq[4]='\0';
		int temnum,temseqi;
		for(i=0;i<3;i++)
		{
			temami[i]=string[17+i];
		}
		temnum=aminoid(temami);
		sltbrgl[numsltbrg].initid=temnum;
		for(i=0;i<4;i++)
		{
			temseq[i]=string[22+i];
		}
		temseqi=atoi(temseq);
		sltbrgl[numsltbrg].initindex=temseqi;
		//end
		for(i=0;i<4;i++)
		{
			sltbrgl[numsltbrg].endatom[i]=string[42+i];
		}
		sltbrgl[numsltbrg].endchainid=string[51];
		for(i=0;i<3;i++)
		{
			temami[i]=string[47+i];
		}
		temnum=aminoid(temami);
		sltbrgl[numsltbrg].endid=temnum;
		for(i=0;i<4;i++)
		{
			temseq[i]=string[52+i];
		}
		temseqi=atoi(temseq);
		sltbrgl[numsltbrg].endindex=temseqi;

		numsltbrg++;
		return 11;
	}
//	CISPEP   2 THR D   92    PRO D   93          0       359.80		
	else if (string[0]=='C' && string[1]=='I' && string[2]=='S' && string[3]=='P'
		&& string[4]=='E' && string[5]=='P') 
	{
		if(numcispep==0)
		{
			cispepl=new cispeplink[alloccispep];
		}
		else if(numcispep==alloccispep-1)
		{
			alloccispep*=2;
			cispepl = (cispeplink*)realloc(cispepl, alloccispep*sizeof(cispeplink)); 		
		}
		//start
		cispepl[numcispep].initchainid=string[15];
		char temnumc[5],temami[3];
		temnumc[4]='\0';
		int temnumi,temamii;
		for(i=0;i<3;i++)
		{
			temami[i]=string[11+i];
		}
		temamii=aminoid(temami);
		cispepl[numcispep].initid=temamii;
		for(i=0;i<4;i++)
		{
			temnumc[i]=string[17+i];
		}
		temnumi=atoi(temnumc);
		cispepl[numcispep].initindex=temnumi;
		//end
		cispepl[numcispep].endchainid=string[29];
		for(i=0;i<3;i++)
		{
			temami[i]=string[25+i];
		}
		temamii=aminoid(temami);
		cispepl[numcispep].endid=temamii;
		for(i=0;i<4;i++)
		{
			temnumc[i]=string[31+i];
		}
		temnumi=atoi(temnumc);
		cispepl[numcispep].endindex=temnumi;

		numcispep++;
		return 12;
	}
	
//	SITE     3 PAA  9 GLN A 231                                             8ATC 385
//	SITE     1 ZNB  4 CYS B 109  CYS B 114  CYS B 138  CYS B 141            8ATC 386		
	else if (string[0]=='S' && string[1]=='I' && string[2]=='T' && string[3]=='E'
		&& string[4]==' ' ) 
	{
		if(numsite==0)
		{
			sitel=new sitelink[allocsite];
		}
		else if(numsite==allocsite-1)
		{
			allocsite*=2;
			sitel = (sitelink*)realloc(sitel, allocsite*sizeof(sitelink)); 		
		}
		//start
		for(i=0;i<3;i++)
		{
			sitel[numsite].sitename[i]=string[11+i];
		}
		char temnumc[3];
		temnumc[2]='\0';
		int temnum;
		for(i=0;i<2;i++)
		{
			temnumc[i]=string[15+i];
		}
		temnum=atoi(temnumc);
		sitel[numsite].sitenum=temnum;
		char temami[3],temseq[5];
		temseq[4]='\0';
		int temamii,temseqi;
		for(i=0;i<4;i++)
		{
			for(j=0;j<3;j++)
			{
				temami[j]=string[18+11*i+j];
			}
			temamii=aminoid(temami);
			sitel[numsite].aminoid[i]=temamii;
			sitel[numsite].chainid[i]=string[22+11*i];
			for(j=0;j<4;j++)
			{
				temseq[j]=string[23+11*i+j];
			}
			temseqi=atoi(temseq);
			sitel[numsite].amiindex[i]=temseqi;
		}
			
		numsite++;
		return 13;
	}
//	MODEL        1		
	else if (string[0]=='M' && string[1]=='O' && string[2]=='D' && string[3]=='E'
		&& string[4]=='L' ) 
	{
		// alloc space
		char tempnum[5];
		tempnum[4]='\0';
		int tempnumi;
		for(i=0;i<4;i++)
		{
			tempnum[i]=string[10+i];
		}
		tempnumi=atoi(tempnum);
		
		if(numpromod==0 && flagfirstmodel)
		{
			flagfirstmodel=false;
			return 42;
		}
		if(numpromod==allocpromod-1)
		{
			allocpromod*=2;
			promod = (promodel*)realloc(promod, allocpromod*sizeof(promodel));
			for(i=numpromod+1;i<allocpromod;i++)
			{
				promod[i].nchain=0;
				for(int j=0;j<maxnumchains;j++)
				{
					promod[i].procha[j].chainseg.init=-1;
					promod[i].procha[j].chainseg.term=-1;
				}
			}
		}
		// end of model
		if(promod[numpromod].procha[promod[numpromod].nchain].chainseg.term==-1)
		{
			promod[numpromod].procha[promod[numpromod].nchain].chainseg.term=numproseq-1;
		}
		flagbeginmodel=true;
		// only here change numpromod
		numpromod++;
		return 14;
	}
	else if (string[0]=='E' && string[1]=='N' && string[2]=='D' && string[3]=='M'
		&& string[4]=='D' && string[5]=='L') 
	{
		// do nothing
		char tempnum[6],tempami[3],tempindex[5],tempid;
		tempnum[5]='\0';
		//tempami[3]='\0';
		tempindex[4]='\0';
		int tempnumi,tempindexi;
		for(i=0;i<5;i++)
		{
			tempnum[i]=string[6+i];
		}
		tempnumi=atoi(tempnum);
		for(i=0;i<3;i++)
		{
			tempami[i]=string[17+i];
		}
		tempid=string[21];
		for(i=0;i<4;i++)
		{
			tempindex[i]=string[22+i];
		}
		
		tempindexi=atoi(tempindex);
		// end of model
		if(promod[numpromod].procha[promod[numpromod].nchain].chainseg.term==-1)
		{
			promod[numpromod].procha[promod[numpromod].nchain].chainseg.term=numproseq-1;
		}
		return 15;
	}
//	HETATM 1415  O2  BLE P   1      13.775  30.147  14.862  1.09 20.95           O
//	TER    1416      BLE P   1  
	else if (string[0]=='T' && string[1]=='E' && string[2]=='R' ) 
	{
		//end a chain
//		flagter=true;
		return 16;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ATOM   1660  CE2 TYR B 205      43.549  -4.115  45.779  1.00 19.60           C 
//	HETATM 2292 2H   HOH   550      23.244   1.464  47.034  1.00  0.00           H 
	else if( (string[0]=='A' && string[1]=='T' && string[2]=='O' && string[3]=='M'
		&& string[4]==' ' ) || (string[0]=='H' && string[1]=='E' && string[2]=='T' 
		&& string[3]=='A' && string[4]=='T' && string[5]=='M') )
	{
		if (string[16]!=' ' && string[16]!='A' && numproseq>0)
		{	
			if(
				proseq[numproseq-1].detailtype[0]==string[12] &&
				proseq[numproseq-1].detailtype[1]==string[13] &&
				proseq[numproseq-1].detailtype[2]==string[14] &&
				proseq[numproseq-1].detailtype[3]==string[15] 
				)
			{
				return 41;		
			}			
		}		
		if(numproseq==0)
		{
			proseq=new atom[allocproseq];
		}
		else if(numproseq==allocproseq-1)
		{
			allocproseq*=2;
			proseq = (atom*)realloc(proseq, allocproseq*sizeof(atom)); 		
		}
		//begin
		if(string[0]=='A' && string[1]=='T' && string[2]=='O' && string[3]=='M')
		{
			proseq[numproseq].simpletype=1;
		}
		else if(string[0]=='H' && string[1]=='E' && string[2]=='T' && string[3]=='A'
		&& string[4]=='T' && string[5]=='M')
		{
			proseq[numproseq].simpletype=2;
		}
		char tempseq[6];
		tempseq[5]='\0';
		//indexnumber
		i=6,j=0;
		for(i=6;i<11;i++)
		{
			tempseq[j++]=string[i];
		}
		proseq[numproseq].seqno=atoi(tempseq);
		//type
		for(j=0;j<4;j++)
		{
			proseq[numproseq].detailtype[j]=' ';
		}
			//start none empty	
		for(i=12;i<16;i++)
		{
			if(string[i]!=' ')
			{
				break;
			}
		}
		//proseq[numproseq].detailtype[4]=i-12;
		proseq[numproseq].detailtype[4]='\0';
		i=12;j=0;
		while(i<16)
		{
			if(string[i]!=' ')
			{
				proseq[numproseq].detailtype[j++]=string[i];
			}
			i++;
		}	
		proseq[numproseq].detail=-1;
		if(proseq[numproseq].detailtype[0]=='C')
		{  
			if(proseq[numproseq].detailtype[1]=='A')
			{
				proseq[numproseq].detail=0;//ca
			
			}       
			else if(proseq[numproseq].detailtype[1]==' ')
			{
				proseq[numproseq].detail=1;//c
			}
			else proseq[numproseq].detail=7;//other cb
		}
		else if(proseq[numproseq].detailtype[0]=='O')
		{
			if(proseq[numproseq].detailtype[1]==' ')
			{
				proseq[numproseq].detail=3;
			}
			else
			{
				proseq[numproseq].detail=11;
			}
		}
		else if(proseq[numproseq].detailtype[0]=='N')
		{
			if(proseq[numproseq].detailtype[1]==' ')
			{
				proseq[numproseq].detail=2;
			}
			else 
			{
				proseq[numproseq].detail=8;//other ne
			}
		}
		else if(proseq[numproseq].detailtype[0]=='S')
		{
			proseq[numproseq].detail=4;
		}
		else if(proseq[numproseq].detailtype[0]=='P')
		{
			proseq[numproseq].detail=6;
		}
		else if(proseq[numproseq].detailtype[0]=='H' 
			|| proseq[numproseq].detailtype[1]=='H'
			|| string[13]=='H')
		{
			if(proseq[numproseq].detailtype[0]=='H' && proseq[numproseq].detailtype[1]==' ')
				proseq[numproseq].detail=5;
			else proseq[numproseq].detail=12;
		}
		else if(string[13]=='F' && string[14]=='E')
		{
			proseq[numproseq].detail=9;
		}
		else
		{
			proseq[numproseq].detail=10;
		}
		proseq[numproseq].alt=string[16];
		//amino
		i=17,j=0;
		for(i=17;i<20;i++)
		{
			proseq[numproseq].residue[j++] =string[i];
		}
		proseq[numproseq].residueid=aminoid(proseq[numproseq].residue);
		//chainid
		i=21;
		proseq[numproseq].chainid=string[i];	
		//seq num
		char tempresnum[5];
		tempresnum[4]='\0';//important added
		i=22,j=0;
		for(i=22;i<26;i++)
		{
			tempresnum[j++]=string[i];
		}
		proseq[numproseq].ins=string[26];
		proseq[numproseq].resno =atoi(tempresnum);
		//sscanf(string+22,"%d",&proseq[numproseq].resno);//both are ok
		/*
		char tempx[8],tempy[8],tempz[8];
		//x
		i=30;j=0;
		for(i=30;i<38;i++)
		{
			tempx[j++]=string[i];
		}
		proseq[numproseq].x=float(atof(tempx));   
		//y
		i=38;j=0;
		for(i=38;i<46;i++)
		{
			tempy[j++]=string[i];
		}
		proseq[numproseq].y=float(atof(tempy));  
		//z
		i=46;j=0;
		for(i=46;i<54;i++)
		{
			tempz[j++]=string[i];
		}
		proseq[numproseq].z=float(atof(tempz)); 
		*/
		sscanf(string+30,"%f %f %f",&proseq[numproseq].x,&proseq[numproseq].y,&proseq[numproseq].z);
		sscanf(string+54,"%f",&proseq[numproseq].occu);
		sscanf(string+60,"%f",&proseq[numproseq].tempe);
		// deal with promod[numpromod]
		if(promod[numpromod].nchain==0 
			&& flagbeginmodel)
		{
			promod[numpromod].procha[promod[numpromod].nchain].chainid=string[21];
			promod[numpromod].procha[promod[numpromod].nchain].chainseg.init=numproseq;	
			flagbeginmodel=false;
		}
		else if(//(flagter)||  
			(string[21]!=proseq[numproseq-1].chainid)
			)
		{
			promod[numpromod].procha[promod[numpromod].nchain].chainseg.term=numproseq-1;
			promod[numpromod].nchain=promod[numpromod].nchain+1;
			promod[numpromod].procha[promod[numpromod].nchain].chainid=string[21];
			promod[numpromod].procha[promod[numpromod].nchain].chainseg.init=numproseq;
		}	
//		if(flagter)
//		{
//			flagter=false;
//		}
		numproseq++;

		return 17;
	}
	
	return 0;
}
void ParsePDB::PCA()
{
	int i;
	double a[9],d[3],v[9];
	point3d tpos;
	for(i=0;i<9;i++)
	{	
		a[i]=0; 
	}
	int totbb=0;
	double pcatran[3],pcamat[9];
	pcatran[0]=0;pcatran[1]=0;pcatran[2]=0;
	for(i=0;i<numbb;i++) 
		if(bb[i].indca!=-1)
	{
		pcatran[0]+=proseq[bb[i].indca].x;
		pcatran[1]+=proseq[bb[i].indca].y;
		pcatran[2]+=proseq[bb[i].indca].z;
		totbb++;
	}
	pcatran[0]/=double(totbb);
	pcatran[1]/=double(totbb);
	pcatran[2]/=double(totbb);
	for(i=0;i<numbb;i++)
		if(bb[i].indca!=-1)
	{
		a[0]+=(proseq[bb[i].indca].x-pcatran[0])*(proseq[bb[i].indca].x-pcatran[0]);
		a[1]+=(proseq[bb[i].indca].x-pcatran[0])*(proseq[bb[i].indca].y-pcatran[1]);
		a[2]+=(proseq[bb[i].indca].x-pcatran[0])*(proseq[bb[i].indca].z-pcatran[2]);
		a[3]=a[1];
		a[4]+=(proseq[bb[i].indca].y-pcatran[1])*(proseq[bb[i].indca].y-pcatran[1]);
		a[5]+=(proseq[bb[i].indca].y-pcatran[1])*(proseq[bb[i].indca].z-pcatran[2]);
		a[6]=a[2];
		a[7]=a[5];
		a[8]+=(proseq[bb[i].indca].z-pcatran[2])*(proseq[bb[i].indca].z-pcatran[2]);
	}
		
	for(i=0;i<9;i++)
	{
		a[i]=a[i]/double(totbb); 
	}	
	jacobi(3,a,d,v);
	double temp;	
	if(d[0]<d[1])
	{
		for(i=0;i<3;i++)
		{
			temp=v[i*3];
			v[i*3]=v[i*3+1];
			v[i*3+1]=temp;
		}
		temp=d[0];
		d[0]=d[1];
		d[1]=temp;
	}
	if(d[0]<d[2])
	{
		for(i=0;i<3;i++)
		{
			temp=v[i*3];
			v[i*3]=v[i*3+2];
			v[i*3+2]=temp;
		}
		temp=d[0];d[0]=d[2];d[2]=temp;
	}
	if(d[1]<d[2])
	{
		for(i=0;i<3;i++)
		{
			temp=v[i*3+1];
			v[i*3+1]=v[i*3+2];
			v[i*3+2]=temp;
		}
		temp=d[1];d[1]=d[2];d[2]=temp;
	}	
	for(i=0;i<9;i++)
	{
		pcamat[i]=v[i];
	}
	for(i=0;i<numproseq;i++)
	{
		tpos.x=(proseq[i].x-pcatran[0])*v[0]+(proseq[i].y-pcatran[1])*v[3]+(proseq[i].z-pcatran[2])*v[6];
		tpos.y=(proseq[i].x-pcatran[0])*v[1]+(proseq[i].y-pcatran[1])*v[4]+(proseq[i].z-pcatran[2])*v[7];
		tpos.z=(proseq[i].x-pcatran[0])*v[2]+(proseq[i].y-pcatran[1])*v[5]+(proseq[i].z-pcatran[2])*v[8];
		proseq[i].x=float(tpos.x);proseq[i].y=float(tpos.y);proseq[i].z=float(tpos.z);
	}
}
bool ParsePDB::loadpdb(char *pdbfile)
{
	FILE	*filein;
	char	oneline[255];	
	bool modelend=false;
    filein = fopen(pdbfile, "rt");
	if(filein==NULL)
	{
		printf( "PDB file can not be opened %s\n", pdbfile);
		return false;
	}
	int i;
	for(i=0;i<5;i++)
	{
		pdbname[i]=pdbfile[i];
	}
	
	//init
	headnumchain=-1;
	indexchain=0;
	for(i=0;i<maxnumchains;i++)
	{
		if(aminoinchain[i]!=NULL)
		{	
			delete[]aminoinchain[i];
			aminoinchain[i]=NULL;
		}
	}
	numhelix=0;numsheet=0;numturn=0;numlink=0;	
	allochelix=30;allocsheet=30;allocturn=30;alloclink=30;
	numssbond=0;numsltbrg=0;numcispep=0;numhydbnd=0;numsite=0;
	allocssbond=30;allocsltbrg=30;alloccispep=30;allochydbnd=30;allocsite=30;
	numpromod=0;allocpromod=1;
	numproseq=0;allocproseq=800;
	pt.ismtrix=false;
	pt.isorigx=false;
	pt.isscale=false;	
	if(helixl!=NULL)
	{
		free(helixl);
		helixl=NULL;
	}
	if(sheetl!=NULL)
	{
		free(sheetl);
		sheetl=NULL;
	}
	if(turnl!=NULL)
	{
		free(turnl);
		turnl=NULL;
	}
	if(linkl!=NULL)
	{
		free(linkl);
		linkl=NULL;
	}
		if(ssbondl!=NULL)
	{
		free(ssbondl);
		ssbondl=NULL;
	}
	if(sltbrgl!=NULL)
	{
		free(sltbrgl);
		sltbrgl=NULL;
	}
	if(cispepl!=NULL)
	{
		free(cispepl);
		cispepl=NULL;
	}
	if(hydbndl!=NULL)
	{
		free(hydbndl);
		hydbndl=NULL;
	}
	if(sitel!=NULL)
	{
		free(sitel);
		sitel=NULL;
	}
	if(promod!=NULL)
	{
		free(promod);
		promod=NULL;
	}
		
	if(proseq!=NULL)
	{
		free(proseq);
		proseq=NULL;
	}
	// allocpromod 1
	promod=new promodel[allocpromod];
	flagfirstmodel=true;
	flagbeginmodel=true;
	for(i=0;i<allocpromod;i++)
	{
		promod[i].nchain=0;
		for(int j=0;j<maxnumchains;j++)
		{
			promod[i].procha[j].chainseg.init=-1;
			promod[i].procha[j].chainseg.term=-1;
		}
	}
	//read
	char oldline[255];oldline[0]='\0';
	while(!feof(filein) && !modelend)
	{
		fgets(oneline,255,filein);
		if(strcmp(oneline,oldline)!=0 || oldline[0]=='\0')
			dealline(oneline);
		strcpy(oldline,oneline);
    }
	fclose(filein);

	//realloc
	if(promod[numpromod].procha[promod[numpromod].nchain].chainseg.term==-1)
	{
		promod[numpromod].procha[promod[numpromod].nchain].chainseg.term=numproseq-1;	
	}
	helixl = (helixlink*)realloc(helixl, numhelix*sizeof(helixlink)); 
	sheetl = (sheetlink*)realloc(sheetl, numsheet*sizeof(sheetlink));
	turnl = (turnlink*)realloc(turnl, numturn*sizeof(turnlink));
	linkl = (linklink*)realloc(linkl, numlink*sizeof(linklink));
	
	ssbondl = (ssbondlink*)realloc(ssbondl, numssbond*sizeof(ssbondlink));
	hydbndl = (hydbndlink*)realloc(hydbndl, numhydbnd*sizeof(hydbndlink));
	sltbrgl = (sltbrglink*)realloc(sltbrgl, numsltbrg*sizeof(sltbrglink));
	cispepl = (cispeplink*)realloc(cispepl, numcispep*sizeof(cispeplink));
	sitel = (sitelink*)realloc(sitel, numsite*sizeof(sitelink));

	proseq = (atom*)realloc(proseq, numproseq*sizeof(atom));
	promod = (promodel*)realloc(promod, (numpromod+1)*sizeof(promodel));

	
	return true;
}
void ParsePDB::splitpdb()
{
	int i,j,k,l;
	FILE *outfile;
	char tmpname[20],tmpn[3];
	for(i=0;i<4;i++)
	{
		tmpname[i]=pdbname[i];
	}
	for (i=0;i<=numpromod;i++)
	{
		for(j=0;j<=promod[i].nchain;j++)
		{
			if(proseq[promod[i].procha[j].chainseg.init].simpletype==1)
			{
				tmpname[4]=promod[i].procha[j].chainid;
				if(tmpname[4]==' ')
				{
					tmpname[4]='A';
				}
				tmpname[5]='\0';
				if(numpromod==0)
				{
					strcat(tmpname,".pdb");
				}
				else if(i<10)
				{
					sprintf(tmpn,"0%d",i);
					strcat(tmpname,tmpn);
					strcat(tmpname,".pdb");
				}
				else
				{
					sprintf(tmpn,"%d",i);
					strcat(tmpname,tmpn);
					strcat(tmpname,".pdb");
				}
				outfile=fopen(tmpname,"wr");
				for(k=promod[i].procha[j].chainseg.init;k<=promod[i].procha[j].chainseg.term;k++)
				{
					if(proseq[k].simpletype==1)
					{
						char tmpatom[4];
						tmpatom[0]=' ';tmpatom[1]=' ';tmpatom[2]=' ';tmpatom[3]=' ';
						for(l=0;l<4-proseq[k].detailtype[4];l++)
						{
							tmpatom[proseq[k].detailtype[4]+l]=proseq[k].detailtype[l];
						}
						fprintf(outfile,"ATOM  %5d %c%c%c%c %c%c%c %c%4d    %8.3f%8.3f%8.3f\n",
							proseq[k].seqno,tmpatom[0],
							tmpatom[1],tmpatom[2],tmpatom[3],proseq[k].residue[0],
							proseq[k].residue[1],proseq[k].residue[2],proseq[k].chainid,
							proseq[k].resno,proseq[k].x,proseq[k].y,proseq[k].z);
					}
				}
				fclose(outfile);
			}
		}
	}
}
int ParsePDB::aminoid(char aminoname)
{
	int i;
	//empty
	if(aminoname==' ')
	{
	//	printf("empty amino name\n");
		return 23;
	}
	// one in 25
	for(i=0;i<25;i++)
	{
		if(aminoname==aad1[i])
		{
			return i;
		}
	}
	// unknown
	//printf("unknown amino name %c%c%c\n",aminoname[0],aminoname[1],aminoname[2]);
	return 23;
}
int ParsePDB::aminoid(char *aminoname)
{
	int i;
	//empty
	if(aminoname[0]==' ' && aminoname[1]==' ' && aminoname[2]==' ')
	{
	//	printf("empty amino name\n");
		return 23;
	}
	// one in 25
	for(i=0;i<25;i++)
	{
		if(aminoname[0]==aad3[i][0] && aminoname[1]==aad3[i][1] && aminoname[2]==aad3[i][2])
		{
			if(i==24)
			{
				i=23;
			}
			return i;
		}
		else if(aminoname[0]==' ' && aminoname[1]==' ' && aminoname[2]==aad1[i])
		{
			return i;
		}
	}
	// unknown
	//printf("unknown amino name %c%c%c\n",aminoname[0],aminoname[1],aminoname[2]);
	return 23;
}
void ParsePDB::extractbb(int model, int chain,int type)
{
	if(chain> promod[model].nchain  || chain<-1)
	{
		printf("no this chain");
		return;
	}
	if(model<0 || model>numpromod)
	{
		printf("no this model");
		return;
	}
	if(type!=1 && type!=3)
	{
		printf("type should be 1 or 3");
		return;
	}
	int i;
	if(bb)
	{
		delete[]bb;
		bb=NULL;
	}
	int allocbb=1+numproseq/4;
	numbb=0;
	bb=new boneinfo[allocbb];
	int seqinit,seqterm;
	if(chain==-1)
	{
		seqinit=promod[model].procha[0].chainseg.init;
		seqterm=promod[model].procha[promod[model].nchain].chainseg.term;
	}
	else
	{
		seqinit=promod[model].procha[chain].chainseg.init;
		seqterm=promod[model].procha[chain].chainseg.term;
	}
	int j,k;
	if(type==1)// ca
	{
		for(i=seqinit;i<=seqterm;i++)
		{
			j=i;
			while(j<=seqterm && proseq[i].resno==proseq[j].resno)
			{
				j++;
			}
			
			bb[numbb].indca=-1;
			for(k=i;k<j;k++)
			{
				if(/*proseq[k].simpletype==1 &&*/ proseq[k].detail==0)
				{
					bb[numbb].indca=k;			
					break;
				}
			}
			bb[numbb].istart=i;
			bb[numbb].iend=j-1;
			bb[numbb].resid=aad1[proseq[i].residueid];
			bb[numbb].resind=proseq[i].resno;
			numbb++;
			if(numbb==allocbb)
			{
				allocbb*=2;
				bb = (boneinfo *)realloc(bb, allocbb*sizeof(boneinfo));
			}
			i=j-1;
		}
		bb = (boneinfo *)realloc(bb, numbb*sizeof(boneinfo));
		
	}
	else if(type==3)//n ca c
	{
		for(i=seqinit;i<=seqterm;i++)
		{
			j=i;
			while(j<=seqterm && proseq[i].resno==proseq[j].resno)
			{
				j++;
			}
			
			bb[numbb].indn=-1;bb[numbb].indca=-1;bb[numbb].indc=-1;bb[numbb].indo=-1;
			for(k=i;k<j;k++)
			{
				if(/*proseq[k].simpletype==1 && */proseq[k].detail==2)
				{
					bb[numbb].indn=k;	
				}
				else if(/*proseq[k].simpletype==1 && */proseq[k].detail==0)
				{
					bb[numbb].indca=k;
				}
				//else 
				else if(/*proseq[k].simpletype==1 && */proseq[k].detail==1)
				{
					bb[numbb].indc=k;
				}
				else if(/*proseq[k].simpletype==1 && */proseq[k].detail==3)
				{
					bb[numbb].indo=k;
				}
			}
			bb[numbb].istart=i;
			bb[numbb].iend=j-1;
			bb[numbb].resid=aad1[proseq[i].residueid];
			bb[numbb].resind=proseq[i].resno;
			numbb++;
			if(numbb==allocbb)
			{
				allocbb*=2;
				bb = (boneinfo *)realloc(bb, allocbb*sizeof(boneinfo));
			}
			i=j-1;
			//notice that i will add 1 in the for()
		}
		bb = (boneinfo *)realloc(bb, numbb*sizeof(boneinfo));
		effbb=0;
		for(i=0;i<numbb;i++)
		{
			if(bb[i].indca!=-1 && bb[i].indc!=-1 && bb[i].indn!=-1 && bb[i].indo!=-1)
			{
				effbb++;
			}
		}
	}
	
	if(numbb!=(proseq[seqterm].resno-proseq[seqinit].resno+1))
	{
	//	printf("Lack some residues [%d,%d]\n",numbb,proseq[seqterm].resno-proseq[seqinit].resno+1);
	}
}
