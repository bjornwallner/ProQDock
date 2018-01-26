/*////////////////////////////////////////////////////////////////
Permission to use, copy, modify, and distribute this program for
any purpose, with or without fee, is hereby granted, provided that
the notices on the head, the reference information, and this
copyright notice appear in all copies or substantial portions of
the Software. It is provided "as is" without express or implied
warranty.
*////////////////////////////////////////////////////////////////
// ProteinSurface.cpp: implementation of the ProteinSurface class.
//
//////////////////////////////////////////////////////////////////////
#include "CommonPara.h"
#include "ProteinSurface.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ProteinSurface::ProteinSurface()
{
	int i;
	boxlength=128;
	flagradius=false;
	scalefactor=1;
	proberadius=1.4;
	for(i=0;i<13;i++)
		depty[i]=NULL;
	vp=NULL;
	pheight=0;
	pwidth=0;
	plength=0;

	faces=NULL;
	verts=NULL;
	facenumber=0;
	vertnumber=0;
	sarea=0;
	svolume=0;
	carea=0;
	cvolume=0;
	ncav=0;
	eachcavityvolume=NULL;
	fixsf=4;
	depval=NULL;
}

ProteinSurface::~ProteinSurface()
{
	int i,j;
	for(i=0;i<13;i++)
	{	
		if(depty[i]!=NULL)
		{
			free(depty[i]);
			depty[i]=NULL;
		}
	}
	if(vp!=NULL)
	{  
		for(i=0;i<plength;i++)
		{   
			for(j=0;j<pwidth;j++)
				free(vp[i][j]);
		}
		for(i=0;i<plength;i++)
			free(vp[i]);
   		free(vp);
		vp=NULL;
	}
	if(faces!=NULL)
	{
		free(faces);
		faces=NULL;
	}
	if(verts!=NULL)
	{
		free(verts);
		verts=NULL;
	}
	if(eachcavityvolume!=NULL)
	{
		free(eachcavityvolume);
		eachcavityvolume=NULL;
	}
	if(depval!=NULL)
	{
		free(depval);
		depval=NULL;
	}
}
void ProteinSurface::calcdepth(int numbb,int seqinit,int seqterm,atom* proseq,boneinfo *bb,bool type)
{
	int i,tind;
	int ox,oy,oz;
	point3d cp;
	if(depval)
	{
		delete[]depval;
	}
	if(!type)
	{
		numdep=numbb;
		depval=new double[numbb];
		for(i=0;i<numbb;i++)
		{
			tind=bb[i].indca;
			cp.x=proseq[tind].x+ptran.x;
			cp.y=proseq[tind].y+ptran.y;
			cp.z=proseq[tind].z+ptran.z;
			cp.x*=scalefactor;
			cp.y*=scalefactor;
			cp.z*=scalefactor;
			ox=int(cp.x+0.5);
			oy=int(cp.y+0.5);
			oz=int(cp.z+0.5);
			depval[i]=vp[ox][oy][oz].distance/scalefactor-proberadius;
			if(depval[i]<rasrad[proseq[tind].detail]) depval[i]=rasrad[proseq[tind].detail];
			depval[i]+=proberadius;
		}
	}
	else
	{
		numdep=seqterm-seqinit+1;
		depval=new double[seqterm-seqinit+1];
		for(i=seqinit;i<=seqterm;i++)
		{
			cp.x=proseq[i].x+ptran.x;
			cp.y=proseq[i].y+ptran.y;
			cp.z=proseq[i].z+ptran.z;
			cp.x*=scalefactor;
			cp.y*=scalefactor;
			cp.z*=scalefactor;
			ox=int(cp.x+0.5);
			oy=int(cp.y+0.5);
			oz=int(cp.z+0.5);
			depval[i-seqinit]=vp[ox][oy][oz].distance/scalefactor-proberadius;
			if(depval[i-seqinit]<rasrad[proseq[i].detail]) depval[i-seqinit]=rasrad[proseq[i].detail];
			depval[i-seqinit]+=proberadius;
		}
	}
}
void ProteinSurface::checkEuler()
{
	int i,j,k;
	int ia,ib,ic;
	int *vertdeg[4][20];//0 a 1 b 2 face 3 cutend]
	bool *vertflag=new bool[int(vertnumber*1.1)];
	int *vertgroup=new int[int(vertnumber*1.1)];
	int *vertnum=new int[int(vertnumber*1.1)];
	for(i=0;i<4;i++)
	{
		for(j=0;j<20;j++)
			vertdeg[i][j]=new int[int(vertnumber*1.1)];
	}
	for(j=0;j<vertnumber;j++)
	{
		vertnum[j]=0;
		vertgroup[j]=0;
		vertflag[j]=true;
	}
	bool *flagface=new bool[facenumber];
	//degree of each vert
	for(i=0;i<facenumber;i++)
	{
		ia=faces[i].a;
		ib=faces[i].b;
		ic=faces[i].c;
		vertdeg[0][vertnum[ia]][ia]=ib;
		vertdeg[1][vertnum[ia]][ia]=ic;
		vertdeg[2][vertnum[ia]][ia]=i;
		vertdeg[0][vertnum[ib]][ib]=ic;
		vertdeg[1][vertnum[ib]][ib]=ia;
		vertdeg[2][vertnum[ib]][ib]=i;
		vertdeg[0][vertnum[ic]][ic]=ia;
		vertdeg[1][vertnum[ic]][ic]=ib;
		vertdeg[2][vertnum[ic]][ic]=i;
		vertnum[ia]++;
		vertnum[ib]++;
		vertnum[ic]++;
		flagface[i]=true;	
	}//i
	int jb,jc;
	int kb,kc;
	int l,m;
	vertinfo *dupvert;
	int numdup=0;
	int allocdup=20;
	dupvert=new vertinfo[allocdup];
	bool flagdup;
	int *tpindex[3];
	for(i=0;i<3;i++)
	{
		tpindex[i]=new int[20];
	}
	for(i=0;i<vertnumber;i++)
	{
		//remove dup faces
		for(j=0;j<vertnum[i]-1;j++)
		{
			jb=vertdeg[0][j][i];
			jc=vertdeg[1][j][i];
			for(k=j+1;k<vertnum[i];k++)
			{
				kb=vertdeg[0][k][i];
				kc=vertdeg[1][k][i];
				if(jb==kc && jc==kb)
				{
		//			printf("%d dup face %d %d [%d %d %d]\n",i,vertdeg[2][j][i],vertdeg[2][k][i],i,jb,jc);
					flagface[vertdeg[2][j][i]]=false;
					flagface[vertdeg[2][k][i]]=false;
					for(l=j;l<k-1;l++)
					{
						vertdeg[0][l][i]=vertdeg[0][l+1][i];
						vertdeg[1][l][i]=vertdeg[1][l+1][i];
						vertdeg[2][l][i]=vertdeg[2][l+1][i];
					}
					for(l=k-1;l<vertnum[i]-2;l++)
					{
						vertdeg[0][l][i]=vertdeg[0][l+2][i];
						vertdeg[1][l][i]=vertdeg[1][l+2][i];
						vertdeg[2][l][i]=vertdeg[2][l+2][i];
					}
					j--;
					k=vertnum[i];
					vertnum[i]-=2;
				}//duplicate
				else if(jb==kb && jc==kc)
				{
		//			printf("wrong same faces %d %d\n",vertdeg[2][j][i],vertdeg[2][k][i]);
				}
			}//k
		}//j
		if(vertnum[i]==0)
		{
	//		printf("no use vertex %d\n",i);
			vertflag[i]=false;
			continue;
		}
		else if(vertnum[i]==1 || vertnum[i]==2)
		{
		//	printf("single vertex %d %d \n",i,vertnum[i]);
		//	vertflag[i]=false;
		}
		//reorder 
		flagdup=false;
		for(j=0;j<vertnum[i]-1;j++)
		{
			for(k=j+1;k<vertnum[i];k++)
			{
				if(vertdeg[0][j][i]==vertdeg[0][k][i])
				{
					flagdup=true;
					break;
				}
			}
			if(flagdup)
				break;
		}
		if(flagdup)
		{
			for(k=j;k<vertnum[i];k++)
			{
				tpindex[0][k-j]=vertdeg[0][k][i];
				tpindex[1][k-j]=vertdeg[1][k][i];
				tpindex[2][k-j]=vertdeg[2][k][i];
			}
			for(k=0;k<j;k++)
			{
				tpindex[0][vertnum[i]-j+k]=vertdeg[0][k][i];
				tpindex[1][vertnum[i]-j+k]=vertdeg[1][k][i];
				tpindex[2][vertnum[i]-j+k]=vertdeg[2][k][i];
			}
			for(k=0;k<vertnum[i];k++)
			{
				vertdeg[0][k][i]=tpindex[0][k];
				vertdeg[1][k][i]=tpindex[1][k];
				vertdeg[2][k][i]=tpindex[2][k];
			}
		}
		//arrage all faces around a vert
		j=0;	
		while(j<vertnum[i])//start cycle
		{		
			jb=vertdeg[0][j][i];
			jc=vertdeg[1][j][i];
			m=j;
			do{//find m+1
				k=vertnum[i];
				for(k=m+1;k<vertnum[i];k++)
				{
					if(vertdeg[0][k][i]==jc)
						break;
				}
				if(k<vertnum[i])
				{
					if(k!=m+1)
					{
						l=vertdeg[0][m+1][i];
						vertdeg[0][m+1][i]=vertdeg[0][k][i];
						vertdeg[0][k][i]=l;
						l=vertdeg[1][m+1][i];
						vertdeg[1][m+1][i]=vertdeg[1][k][i];
						vertdeg[1][k][i]=l;
						l=vertdeg[2][m+1][i];
						vertdeg[2][m+1][i]=vertdeg[2][k][i];
						vertdeg[2][k][i]=l;
					}
					jc=vertdeg[1][m+1][i];	
					m++;
				}
				else
				{
					break;
				}
			}while(jc!=jb && m<vertnum[i]);
			if(jc==jb)//one cycle
			{
				vertdeg[3][vertgroup[i]][i]=m;
				vertgroup[i]++;
			}
			else//single
			{
		//		printf("no corre index %d %d\n",i,jc);
				vertdeg[3][vertgroup[i]][i]=m;
				vertgroup[i]++;		
				for(j=0;j<vertnum[i];j++)
				{
		//			printf("detail %d %d %d %d %d\n",j,vertdeg[0][j][i],vertdeg[1][j][i],vertdeg[2][j][i],vertdeg[3][j][i]);
				}
			}
			j=m+1;
		}//while
		if(vertgroup[i]!=1)
		{
	//		printf("split vert %d %d\n",i,vertgroup[i]);
	//		for(j=0;j<vertnum[i];j++)
	//		{
	//			printf("%d %d %d %d %d\n",j,vertdeg[0][j][i],vertdeg[1][j][i],vertdeg[2][j][i],vertdeg[3][j][i]);
	//		}
			if(numdup+vertgroup[i]>allocdup)
			{
				allocdup*=2;
				dupvert=(vertinfo *)realloc(dupvert,allocdup*sizeof(vertinfo));
			}
			
			for(j=1;j<vertgroup[i];j++)
			{	
				dupvert[numdup]=verts[i];
				vertflag[numdup+vertnumber]=true;
				vertgroup[numdup+vertnumber]=1;
				vertnum[numdup+vertnumber]=vertdeg[3][j][i]-vertdeg[3][j-1][i];
				for(k=0;k<vertnum[numdup+vertnumber];k++)
				{
					vertdeg[0][k][numdup+vertnumber]=vertdeg[0][vertdeg[3][j-1][i]+k+1][numdup+vertnumber];
					vertdeg[1][k][numdup+vertnumber]=vertdeg[1][vertdeg[3][j-1][i]+k+1][numdup+vertnumber];
					vertdeg[2][k][numdup+vertnumber]=vertdeg[2][vertdeg[3][j-1][i]+k+1][numdup+vertnumber];
				}
				for(k=vertdeg[3][j-1][i]+1;k<=vertdeg[3][j][i];k++)
				{
		//			printf("changing %d %d\n",k,vertdeg[2][k][i]);
					if(faces[vertdeg[2][k][i]].a==i)
					{
						faces[vertdeg[2][k][i]].a=numdup+vertnumber;
						m=faces[vertdeg[2][k][i]].b;
						for(l=0;l<vertnum[m];l++)
						{
							if(vertdeg[2][l][m]==vertdeg[2][k][i])
							{
								if(vertdeg[0][l][m]==i)
								{
									vertdeg[0][l][m]=numdup+vertnumber;
								}
								else if(vertdeg[1][l][m]==i)
								{
									vertdeg[1][l][m]=numdup+vertnumber;
								}
								else 
								{
							//		printf("wrong modified vertab %d\n",m);
								}
							}
						}
						m=faces[vertdeg[2][k][i]].c;
						for(l=0;l<vertnum[m];l++)
						{
							if(vertdeg[2][l][m]==vertdeg[2][k][i])
							{
								if(vertdeg[0][l][m]==i)
								{
									vertdeg[0][l][m]=numdup+vertnumber;
								}
								else if(vertdeg[1][l][m]==i)
								{
									vertdeg[1][l][m]=numdup+vertnumber;
								}
								else 
								{
									printf("wrong modified vertac %d\n",m);
								}
							}
						}
					}
					else if(faces[vertdeg[2][k][i]].b==i)
					{
						faces[vertdeg[2][k][i]].b=numdup+vertnumber;
						m=faces[vertdeg[2][k][i]].a;
						for(l=0;l<vertnum[m];l++)
						{
							if(vertdeg[2][l][m]==vertdeg[2][k][i])
							{
								if(vertdeg[0][l][m]==i)
								{
									vertdeg[0][l][m]=numdup+vertnumber;
								}
								else if(vertdeg[1][l][m]==i)
								{
									vertdeg[1][l][m]=numdup+vertnumber;
								}
								else 
								{
							//		printf("wrong modified vertba %d\n",m);
								}
							}
						}
						m=faces[vertdeg[2][k][i]].c;
						for(l=0;l<vertnum[m];l++)
						{
							if(vertdeg[2][l][m]==vertdeg[2][k][i])
							{
								if(vertdeg[0][l][m]==i)
								{
									vertdeg[0][l][m]=numdup+vertnumber;
								}
								else if(vertdeg[1][l][m]==i)
								{
									vertdeg[1][l][m]=numdup+vertnumber;
								}
								else 
								{
							//		printf("wrong modified vertbc %d\n",m);
								}
							}
						}
					}
					else if(faces[vertdeg[2][k][i]].c==i)
					{
						faces[vertdeg[2][k][i]].c=numdup+vertnumber;
						m=faces[vertdeg[2][k][i]].a;
						for(l=0;l<vertnum[m];l++)
						{
							if(vertdeg[2][l][m]==vertdeg[2][k][i])
							{
								if(vertdeg[0][l][m]==i)
								{
									vertdeg[0][l][m]=numdup+vertnumber;
								}
								else if(vertdeg[1][l][m]==i)
								{
									vertdeg[1][l][m]=numdup+vertnumber;
								}
								else 
								{
							//		printf("wrong modified vertca %d\n",m);
								}
							}
						}
						m=faces[vertdeg[2][k][i]].b;
						for(l=0;l<vertnum[m];l++)
						{
							if(vertdeg[2][l][m]==vertdeg[2][k][i])
							{
								if(vertdeg[0][l][m]==i)
								{
									vertdeg[0][l][m]=numdup+vertnumber;
								}
								else if(vertdeg[1][l][m]==i)
								{
									vertdeg[1][l][m]=numdup+vertnumber;
								}
								else 
								{
						//			printf("wrong modified vertcb %d\n",m);
								}
							}
						}
					}
					else
					{
					//	printf("wrong vert %d face %d [%d %d %d]\n",i,vertdeg[2][k][i],faces[vertdeg[2][k][i]].a,
					//		faces[vertdeg[2][k][i]].b,faces[vertdeg[2][k][i]].c);
					}
				}//k
				vertgroup[i]=1;
				numdup++;
			}//j  
		}//if need to split
	}//i
	for(i=0;i<3;i++)
	{
		delete[]tpindex[i];
	}
	//reduce face
	int totfacenew=0;
	for(i=0;i<facenumber;i++)
	{
		if(flagface[i] && totfacenew!=i)
		{
			faces[totfacenew]=faces[i];
			totfacenew++;
		}
		else if(flagface[i])
		{
			totfacenew++;
		}
	}
//	printf("number faces from %d to %d\n",facenumber,totfacenew);
	facenumber=totfacenew;
	
	//new points
	int totvertnew=0;
	int *vertindex2=new int[vertnumber+numdup];
	verts=(vertinfo*)realloc(verts,(vertnumber+numdup)*sizeof(vertinfo));
	for(i=0;i<vertnumber;i++)
	{
		if(vertflag[i] && totvertnew!=i)
		{
			vertindex2[i]=totvertnew;
			verts[totvertnew]=verts[i];
			totvertnew++;
		}
		else if(vertflag[i])
		{
			vertindex2[i]=totvertnew;
			totvertnew++;	
		}
	}
	for(i=0;i<numdup;i++)
	{
		vertindex2[vertnumber+i]=totvertnew;
		verts[totvertnew]=dupvert[i];
		totvertnew++;
	}
	for(i=0;i<facenumber;i++)
	{
		faces[i].a=vertindex2[faces[i].a];
		faces[i].b=vertindex2[faces[i].b];
		faces[i].c=vertindex2[faces[i].c];
	}
	delete[]vertindex2;
//	printf("number verts from %d to %d (new added %d)\n",vertnumber,totvertnew,numdup);
	vertnumber=totvertnew;
	if((2*vertnumber-facenumber)%4!=0)
	printf("euler num %d\n",2*vertnumber-facenumber);//comp+cav-genus

	//release
	for(i=0;i<4;i++)
	{
		for(j=0;j<20;j++)
			delete[]vertdeg[i][j];
	}
	delete[]vertgroup;
	delete[]vertflag;
	delete[]vertnum;
	delete[]flagface;
	delete[]dupvert;
}
void ProteinSurface::laplaciansmooth(int numiter)
{
	point3d *tps=new point3d[vertnumber];
	int *vertdeg[20];
	int i,j;
	bool flagvert;
	for(i=0;i<20;i++)
	{
		vertdeg[i]=new int[vertnumber];		
	}
	for(i=0;i<vertnumber;i++)
	{
		vertdeg[0][i]=0;
	}
	for(i=0;i<facenumber;i++)
	{
		//a
		flagvert=true;
		for(j=0;j<vertdeg[0][faces[i].a];j++)
		{
			if(faces[i].b==vertdeg[j+1][faces[i].a])
			{
				flagvert=false;
				break;
			}
		}
		if(flagvert)
		{
			vertdeg[0][faces[i].a]++;
			vertdeg[vertdeg[0][faces[i].a]][faces[i].a]=faces[i].b;
			
		}
		flagvert=true;
		for(j=0;j<vertdeg[0][faces[i].a];j++)
		{
			if(faces[i].c==vertdeg[j+1][faces[i].a])
			{
				flagvert=false;
				break;
			}
		}
		if(flagvert)
		{
			vertdeg[0][faces[i].a]++;
			vertdeg[vertdeg[0][faces[i].a]][faces[i].a]=faces[i].c;
			
		}
		//b
		flagvert=true;
		for(j=0;j<vertdeg[0][faces[i].b];j++)
		{
			if(faces[i].a==vertdeg[j+1][faces[i].b])
			{
				flagvert=false;
				break;
			}
		}
		if(flagvert)
		{
			vertdeg[0][faces[i].b]++;
			vertdeg[vertdeg[0][faces[i].b]][faces[i].b]=faces[i].a;
			
		}
		flagvert=true;
		for(j=0;j<vertdeg[0][faces[i].b];j++)
		{
			if(faces[i].c==vertdeg[j+1][faces[i].b])
			{
				flagvert=false;
				break;
			}
		}
		if(flagvert)
		{
			vertdeg[0][faces[i].b]++;
			vertdeg[vertdeg[0][faces[i].b]][faces[i].b]=faces[i].c;
			
		}
		//c
		flagvert=true;
		for(j=0;j<vertdeg[0][faces[i].c];j++)
		{
			if(faces[i].a==vertdeg[j+1][faces[i].c])
			{
				flagvert=false;
				break;
			}
		}
		if(flagvert)
		{
			vertdeg[0][faces[i].c]++;
			vertdeg[vertdeg[0][faces[i].c]][faces[i].c]=faces[i].a;
			
		}
		flagvert=true;
		for(j=0;j<vertdeg[0][faces[i].c];j++)
		{
			if(faces[i].b==vertdeg[j+1][faces[i].c])
			{
				flagvert=false;
				break;
			}
		}
		if(flagvert)
		{
			vertdeg[0][faces[i].c]++;
			vertdeg[vertdeg[0][faces[i].c]][faces[i].c]=faces[i].b;
			
		}
	}
	
	double wt=1.00;
	double wt2=0.50;
	int ssign;
	int k;
	double outwt=0.75/(scalefactor+3.5);//area-preserving
for(k=0;k<numiter;k++)
{
	for(i=0;i<vertnumber;i++)
	{
		if(vertdeg[0][i]<3)
		{
			tps[i].x=verts[i].x;
			tps[i].y=verts[i].y;
			tps[i].z=verts[i].z;
		}
		else if(vertdeg[0][i]==3 || vertdeg[0][i]==4)
		{
			tps[i].x=0;
			tps[i].y=0;
			tps[i].z=0;
			for(j=0;j<vertdeg[0][i];j++)
			{
				tps[i].x+=verts[vertdeg[j+1][i]].x;
				tps[i].y+=verts[vertdeg[j+1][i]].y;
				tps[i].z+=verts[vertdeg[j+1][i]].z;
			}
			tps[i].x+=wt2*verts[i].x;
			tps[i].y+=wt2*verts[i].y;
			tps[i].z+=wt2*verts[i].z;
			tps[i].x/=float(wt2+vertdeg[0][i]);
			tps[i].y/=float(wt2+vertdeg[0][i]);
			tps[i].z/=float(wt2+vertdeg[0][i]);
		}
		else
		{
			tps[i].x=0;
			tps[i].y=0;
			tps[i].z=0;
			for(j=0;j<vertdeg[0][i];j++)
			{
				tps[i].x+=verts[vertdeg[j+1][i]].x;
				tps[i].y+=verts[vertdeg[j+1][i]].y;
				tps[i].z+=verts[vertdeg[j+1][i]].z;
			}
			tps[i].x+=wt*verts[i].x;
			tps[i].y+=wt*verts[i].y;
			tps[i].z+=wt*verts[i].z;
			tps[i].x/=float(wt+vertdeg[0][i]);
			tps[i].y/=float(wt+vertdeg[0][i]);
			tps[i].z/=float(wt+vertdeg[0][i]);
		}
	}
	for(i=0;i<vertnumber;i++)
	{
		verts[i].x=tps[i].x;
		verts[i].y=tps[i].y;
		verts[i].z=tps[i].z;
	}
	computenorm();
	for(i=0;i<vertnumber;i++)
	{
		if(verts[i].inout) ssign=1;
		else ssign=-1;
		verts[i].x+=ssign*outwt*verts[i].pn.x;
		verts[i].y+=ssign*outwt*verts[i].pn.y;
		verts[i].z+=ssign*outwt*verts[i].pn.z;
	}
}
	delete[]tps;
	for(i=0;i<20;i++)
		delete[]vertdeg[i];

}

////////////////////////////////////////////////////////////////////////true: heavy atoms
void ProteinSurface::boundbox(int seqinit,int seqterm,atom* proseq,bool atomtype,
							  point3d *minp,point3d *maxp)
{
	int i;
	minp->x=100000;minp->y=100000;minp->z=100000;
	maxp->x=-100000;maxp->y=-100000;maxp->z=-100000;
	for(i=seqinit;i<=seqterm;i++)
	{
		if(proseq[i].simpletype==1 && proseq[i].ins==' ')
		{
			if(atomtype && (proseq[i].detail==5 ||proseq[i].detail==12))
				continue;
			if(proseq[i].x<minp->x)
				minp->x=proseq[i].x;
			if(proseq[i].y<minp->y)
				minp->y=proseq[i].y;
			if(proseq[i].z<minp->z)
				minp->z=proseq[i].z;
			if(proseq[i].x>maxp->x)
				maxp->x=proseq[i].x;
			if(proseq[i].y>maxp->y)
				maxp->y=proseq[i].y;
			if(proseq[i].z>maxp->z)
				maxp->z=proseq[i].z;
		}
	}

}
//label of atom
void ProteinSurface::atomsinout(int seqinit,int seqterm,atom* proseq)
{
	int i;
	for(i=seqinit;i<=seqterm;i++)
	{
		proseq[i].inout=2;//between
	}
	for(i=0;i<vertnumber;i++)
	{
		if(!verts[i].iscont)
			proseq[verts[i].atomid].inout=1;//outer
	}
	for(i=0;i<facenumber;i++)
	{
		if(faces[i].inout)
		{
			proseq[verts[faces[i].a].atomid].inout=3;//inner
			proseq[verts[faces[i].b].atomid].inout=3;
			proseq[verts[faces[i].c].atomid].inout=3;
		}
	}
}
void ProteinSurface::checkinoutpropa()
{
	int i,j,ia,ib,ic;
	int *vertdeg[40];
	for(i=0;i<40;i++)
	{
		vertdeg[i]=new int[vertnumber];
		memset(vertdeg[i],0,vertnumber*sizeof(int));
	}
	for(i=0;i<facenumber;i++)
	{
		ia=faces[i].a;
		ib=faces[i].b;
		ic=faces[i].c;
		vertdeg[vertdeg[39][ia]][ia]=ib;
		vertdeg[39][ia]++;
		vertdeg[vertdeg[39][ia]][ia]=ic;
		vertdeg[39][ia]++;
		vertdeg[vertdeg[39][ib]][ib]=ic;
		vertdeg[39][ib]++;
		vertdeg[vertdeg[39][ib]][ib]=ia;
		vertdeg[39][ib]++;
		vertdeg[vertdeg[39][ic]][ic]=ia;
		vertdeg[39][ic]++;	
		vertdeg[vertdeg[39][ic]][ic]=ib;
		vertdeg[39][ic]++;	
	}//i
	int *arrin=new int[vertnumber];
	int narrin,narrout;
	int *arrout=new int[vertnumber];
	bool *flagpoints=new bool[vertnumber];
	memset(flagpoints,false,vertnumber*sizeof(bool));
	narrin=3;
	arrin[0]=faces[0].a;
	arrin[1]=faces[0].b;
	arrin[2]=faces[0].c;
	int niter=0;
	while(narrin>0)
	{
	//	printf("do iteration %d %d\n",niter,narrin);
		narrout=0;
		for(i=0;i<narrin;i++) if(!flagpoints[arrin[i]])
		{
			flagpoints[arrin[i]]=true;
			for(j=0;j<vertdeg[39][arrin[i]];j++) if(!flagpoints[vertdeg[j][arrin[i]]])
			{
				arrout[narrout]=vertdeg[j][arrin[i]];
				narrout++;
			}
		}
		memcpy(arrin,arrout,vertnumber*sizeof(int));
		narrin=narrout;
		niter++;
	}
	for(i=0;i<vertnumber;i++)
	{
		if(flagpoints[i]) verts[i].inout=true;
		else verts[i].inout=false;
	}
	for(i=0;i<facenumber;i++)
	{
		if(flagpoints[faces[i].a] || flagpoints[faces[i].b] || flagpoints[faces[i].c]) faces[i].inout=false;
		else faces[i].inout=true;
	}
	for(i=0;i<40;i++)
	{
		delete[]vertdeg[i];
	}
	delete[]flagpoints;
	delete[]arrin;
	delete[]arrout;
}
void ProteinSurface::outputply(char *filename,atom* proseq,int ncolor,int tinout)
{//0 both 1 outer 2 inner
	int i;
	unsigned char chaincolor[256];
	int tchain,indcolor2=1;
	char filename2[256];
	char filename3[256];
	sprintf(filename2,"%s.surf",filename);
	sprintf(filename3,"%s.pdb",filename);
	for(i=0;i<256;i++)
	{
		chaincolor[i]=0;
	}
	FILE *file,*file42,*file43;
	file=fopen(filename,"wt");
	file42=fopen(filename2,"wt");
	file43=fopen(filename3,"wt");
	if(file==NULL)
	{
		printf("wrong to output ply file %s\n",filename);
		return;
	}
int *vertind=new int[vertnumber];
bool *flagface=new bool[facenumber];
int realvertnum,realfacenum;
memset(vertind,-1,vertnumber*sizeof(int));
memset(flagface,false,facenumber*sizeof(bool));
if(tinout==0)
{
	realvertnum=vertnumber;
	for(i=0;i<vertnumber;i++)
	{
		vertind[i]=i;
	}
	realfacenum=facenumber;
	memset(flagface,true,facenumber*sizeof(bool));
}
else if(tinout==1)
{
	realvertnum=0;
	for(i=0;i<vertnumber;i++)
	{
		if(verts[i].inout) 
		{
			vertind[i]=realvertnum;
			realvertnum++;
		}
	}
	realfacenum=0;
	for(i=0;i<facenumber;i++)
	{
		if(vertind[faces[i].a]!=-1 && vertind[faces[i].b]!=-1 && vertind[faces[i].c]!=-1)
		{
			realfacenum++;
			flagface[i]=true;
		}
	}
}
else if(tinout==2)
{
	realvertnum=0;
	for(i=0;i<vertnumber;i++)
	{
		if(!verts[i].inout) 
		{
			vertind[i]=realvertnum;
			realvertnum++;
		}
	}
	realfacenum=0;
	for(i=0;i<facenumber;i++)
	{
		if(vertind[faces[i].a]!=-1 && vertind[faces[i].b]!=-1 && vertind[faces[i].c]!=-1)
		{
			realfacenum++;
			flagface[i]=true;
		}
	}
}
	fprintf(file,"ply\n" );
	fprintf(file,"format ascii 1.0\n" );
	fprintf(file,"comment ball mesh\n" );
	fprintf(file,"element vertex %d\n", realvertnum);
	fprintf(file,"property float x\n" );
	fprintf(file,"property float y\n" );
	fprintf(file,"property float z\n" );
	fprintf(file,"property uchar red\n" );
	fprintf(file,"property uchar green\n" );
	fprintf(file,"property uchar blue\n" );
	fprintf(file,"element face %d\n",realfacenum );
	fprintf(file,"property list uchar int vertex_indices\n" );
	fprintf(file,"property uchar red\n" );
	fprintf(file,"property uchar green\n" );
	fprintf(file,"property uchar blue\n" );
	fprintf(file,"end_header\n" );
	int tcolor[3];
    for(i=0;i<vertnumber;i++) if(vertind[i]!=-1)
	{ 
		if(ncolor==0)//pure
		{
			tcolor[0]=mycolor[13][0];
			tcolor[1]=mycolor[13][1];
			tcolor[2]=mycolor[13][2];
		}
		else if(ncolor==1)//atom
		{
			tcolor[0]=color[proseq[verts[i].atomid].detail][0];
			tcolor[1]=color[proseq[verts[i].atomid].detail][1];
			tcolor[2]=color[proseq[verts[i].atomid].detail][2];
			if(verts[i].iscont)
			{
				tcolor[0]=color[14][0];
				tcolor[1]=color[14][1];
				tcolor[2]=color[14][2];
			}
		}
		else if(ncolor==2)//chain
		{
			if(chaincolor[proseq[verts[i].atomid].chainid]==0)
			{
				chaincolor[proseq[verts[i].atomid].chainid]=indcolor2;
				indcolor2++;
				if(indcolor2>18)
					indcolor2=1;
			}
			tcolor[0]=mycolor[chaincolor[proseq[verts[i].atomid].chainid]][0];
			tcolor[1]=mycolor[chaincolor[proseq[verts[i].atomid].chainid]][1];
			tcolor[2]=mycolor[chaincolor[proseq[verts[i].atomid].chainid]][2];
		}
		//proseq[verts[i].atomid].detailtype[4]='\0';
		fprintf(file,"%.3f %.3f %.3f %3d %3d %3d\n", 
			verts[i].x/scalefactor-ptran.x,verts[i].y/scalefactor-ptran.y,
			verts[i].z/scalefactor-ptran.z,tcolor[0],tcolor[1],tcolor[2]);
		fprintf(file42,"%s %c%c%c %4d %c %8.3f %8.3f %8.3f %3d %3d %3d\n", 
			proseq[verts[i].atomid].detailtype,
			proseq[verts[i].atomid].residue[0],
			proseq[verts[i].atomid].residue[1],
			proseq[verts[i].atomid].residue[2],
			proseq[verts[i].atomid].resno,
			proseq[verts[i].atomid].chainid,
			verts[i].x/scalefactor-ptran.x,verts[i].y/scalefactor-ptran.y,
			verts[i].z/scalefactor-ptran.z,tcolor[0],tcolor[1],tcolor[2]);
		fprintf(file43,"%s         %s %c%c%c %c   %3d    %8.3f%8.3f%8.3f\n",
                        "ATOM", 
			proseq[verts[i].atomid].detailtype,
			proseq[verts[i].atomid].residue[0],
			proseq[verts[i].atomid].residue[1],
			proseq[verts[i].atomid].residue[2],
			proseq[verts[i].atomid].chainid,
			proseq[verts[i].atomid].resno,
			verts[i].x/scalefactor-ptran.x,verts[i].y/scalefactor-ptran.y,
			verts[i].z/scalefactor-ptran.z,tcolor[0],tcolor[1],tcolor[2]);
	}
	for(i=0;i<facenumber;i++) if(flagface[i])
	{ 
		if(ncolor==0)//pure
		{
			tcolor[0]=mycolor[13][0];
			tcolor[1]=mycolor[13][1];
			tcolor[2]=mycolor[13][2];
		}
		else if(ncolor==1)//atom
		{
			if(proseq[verts[faces[i].b].atomid].detail==proseq[verts[faces[i].c].atomid].detail)
			{
				tcolor[0]=color[proseq[verts[faces[i].b].atomid].detail][0];
				tcolor[1]=color[proseq[verts[faces[i].b].atomid].detail][1];
				tcolor[2]=color[proseq[verts[faces[i].b].atomid].detail][2];
			}
			else
			{
				tcolor[0]=color[proseq[verts[faces[i].a].atomid].detail][0];
				tcolor[1]=color[proseq[verts[faces[i].a].atomid].detail][1];
				tcolor[2]=color[proseq[verts[faces[i].a].atomid].detail][2];
			}
		}
		else if(ncolor==2)//chain
		{
			if(chaincolor[proseq[verts[faces[i].a].atomid].chainid]==0)
			{
				chaincolor[proseq[verts[faces[i].a].atomid].chainid]=indcolor2;
				indcolor2++;
				if(indcolor2>18)
					indcolor2=1;
			}
			if(chaincolor[proseq[verts[faces[i].b].atomid].chainid]==0)
			{
				chaincolor[proseq[verts[faces[i].b].atomid].chainid]=indcolor2;
				indcolor2++;
				if(indcolor2>18)
					indcolor2=1;
			}
			if(chaincolor[proseq[verts[faces[i].c].atomid].chainid]==0)
			{
				chaincolor[proseq[verts[faces[i].c].atomid].chainid]=indcolor2;
				indcolor2++;
				if(indcolor2>18)
					indcolor2=1;
			}
			if(proseq[verts[faces[i].b].atomid].chainid==proseq[verts[faces[i].c].atomid].chainid)
				tchain=chaincolor[proseq[verts[faces[i].b].atomid].chainid];
			else 
				tchain=chaincolor[proseq[verts[faces[i].a].atomid].chainid];
			tcolor[0]=mycolor[tchain][0];
			tcolor[1]=mycolor[tchain][1];
			tcolor[2]=mycolor[tchain][2];
		}
		if(!faces[i].inout)//outer
			fprintf(file,"3 %d %d %d %3d %3d %3d\n", vertind[faces[i].a],vertind[faces[i].b],vertind[faces[i].c],tcolor[0],tcolor[1],tcolor[2]);
		else
			fprintf(file,"3 %d %d %d %3d %3d %3d\n", vertind[faces[i].a],vertind[faces[i].c],vertind[faces[i].b],tcolor[0],tcolor[1],tcolor[2]);
	}		
	fclose(file);
	delete[]vertind;
	delete[]flagface;
}
void ProteinSurface::outputoff(char *filename) 
{
	// TODO: Add your command handler code here
	FILE  *stream;
	int i;

    if( (stream =fopen(filename,"w+t"))== NULL)
	{
		printf("wrong to output off file %s\n",filename);
		return;
	}
	fprintf(stream,"off\n" );
	fprintf(stream,"%d  %d  0\n",vertnumber,facenumber);
    for(i=0;i<vertnumber;i++)
	{ 
		fprintf(stream,"%.6f %.6f %.6f\n", verts[i].x/scalefactor-ptran.x,
			verts[i].y/scalefactor-ptran.y,
			verts[i].z/scalefactor-ptran.z);	
	}
	for(i=0;i<facenumber;i++)
	{ 
		fprintf(stream,"3 %d %d %d\n", faces[i].a,faces[i].c,faces[i].b);		
	}		
    fclose(stream);
}
void ProteinSurface::buildboundary()
{
	int i,j,k;
	int ii;
	bool flagbound;
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pheight;j++)
		{
			for(k=0;k<pwidth;k++)
			{
				if(vp[i][k][j].inout)
				{
					//6 neighbors
//					if(( k-1>-1 && !vp[i][k-1][j].inout) || ( k+1<pwidth &&!vp[i][k+1][j].inout)
//					|| ( j-1>-1 && !vp[i][k][j-1].inout) || ( j+1<pheight &&!vp[i][k][j+1].inout)
//					|| ( i-1>-1 && !vp[i-1][k][j].inout) || ( i+1<plength &&!vp[i+1][k][j].inout))
//						vp[i][k][j].isbound=true;
			//	/*
					//26 neighbors
					flagbound=false;
					ii=0;
					while(!flagbound && ii<26)
					{
						if(i+nb[ii][0]>-1 && i+nb[ii][0]<plength
							&& k+nb[ii][1]>-1 && k+nb[ii][1]<pwidth
							&& j+nb[ii][2]>-1 && j+nb[ii][2]<pheight
							&& !vp[i+nb[ii][0]][k+nb[ii][1]][j+nb[ii][2]].inout)
						{
							vp[i][k][j].isbound=true;
							flagbound=true;
						}
						else ii++;
					}
			//		*/
				}			
			}
		
		}
	}
}
//determine each voxel
void ProteinSurface::surfaceinterior()
{
	int i,j,k;
	voxel2 *inarray,*outarray;
//	int allocin=1000;
//	int allocout=1000;
	int innum=0;
	int outnum;
	int totnum=0;
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vp[i][j][k].inout=true;
				vp[i][j][k].isbound=false;//has put into array
				if(!vp[i][j][k].isdone)
					totnum++;
			}
		}
	}
	inarray=new voxel2[totnum];
	outarray=new voxel2[totnum];
	if(!vp[0][0][0].isdone)
	{
		inarray[innum].ix=0;
		inarray[innum].iy=0;
		inarray[innum].iz=0;
		innum++;
	}
	else if(!vp[plength-1][0][0].isdone)
	{
		inarray[innum].ix=plength-1;
		inarray[innum].iy=0;
		inarray[innum].iz=0;
		innum++;
	}
	else if(!vp[0][pwidth-1][0].isdone)
	{
		inarray[innum].ix=0;
		inarray[innum].iy=pwidth-1;
		inarray[innum].iz=0;
		innum++;
	}
	else if(!vp[0][0][pheight-1].isdone)
	{
		inarray[innum].ix=0;
		inarray[innum].iy=0;
		inarray[innum].iz=pheight-1;
		innum++;
	}
	else if(!vp[plength-1][pwidth-1][0].isdone)
	{
		inarray[innum].ix=plength-1;
		inarray[innum].iy=pwidth-1;
		inarray[innum].iz=0;
		innum++;
	}
	else if(!vp[0][pwidth-1][pheight-1].isdone)
	{
		inarray[innum].ix=0;
		inarray[innum].iy=pwidth-1;
		inarray[innum].iz=pheight-1;
		innum++;
	}
	else if(!vp[plength-1][0][pheight-1].isdone)
	{
		inarray[innum].ix=plength-1;
		inarray[innum].iy=0;
		inarray[innum].iz=pheight-1;
		innum++;
	}
	else if(!vp[plength-1][pwidth-1][pheight-1].isdone)
	{
		inarray[innum].ix=plength-1;
		inarray[innum].iy=pwidth-1;
		inarray[innum].iz=pheight-1;
		innum++;
	}
	else
	{
		printf("No init point\n");
		return;
	}
	vp[inarray[0].ix][inarray[0].iy][inarray[0].iz].isbound=true;
	int ii,jj,kk;
	voxel2 *tarray,tnv;
	while(innum!=0)
	{
		outnum=0;
		for(i=0;i<innum;i++)
		{
			vp[inarray[i].ix][inarray[i].iy][inarray[i].iz].inout=false;
			for(ii=-1;ii<2;ii++)
			{
				for(jj=-1;jj<2;jj++)
				{
					for(kk=-1;kk<2;kk++)
					{
						tnv.ix=inarray[i].ix+ii;
						tnv.iy=inarray[i].iy+jj;
						tnv.iz=inarray[i].iz+kk;
						if(tnv.ix>-1 && tnv.ix<plength 
							&& tnv.iy>-1 && tnv.iy<pwidth
							&& tnv.iz>-1 && tnv.iz<pheight
							&& vp[tnv.ix][tnv.iy][tnv.iz].inout
							&& !vp[tnv.ix][tnv.iy][tnv.iz].isdone
							&& !vp[tnv.ix][tnv.iy][tnv.iz].isbound)
						{
							vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
							outarray[outnum].ix=tnv.ix;
							outarray[outnum].iy=tnv.iy;
							outarray[outnum].iz=tnv.iz;
							outnum++;
						}
					}//kk
				}//jj
			}//ii
		}
		tarray=outarray;
		outarray=inarray;
		inarray=tarray;
		innum=outnum;
	}//while
	delete[]inarray;
	delete[]outarray;
}
void ProteinSurface::outsas(int numbb,boneinfo *bb,atom* proseq,char *filename)
{
	double *areasum=new double[numbb];
	int i,j;
	int indres;
	for(i=0;i<numbb;i++)
	{
		areasum[i]=0;
	}
	for(i=0;i<facenumber;i++)
	{
		if(!faces[i].inout)
		{
			for(j=0;j<numbb;j++)
			{
				if(verts[faces[i].a].atomid>=bb[j].istart
					&& verts[faces[i].a].atomid<=bb[j].iend)
				{
					indres=j;
					break;
				}
			}	
			areasum[indres]+=faces[i].area/3.0/scalefactor/scalefactor;
			for(j=0;j<numbb;j++)
			{
				if(verts[faces[i].b].atomid>=bb[j].istart
					&& verts[faces[i].b].atomid<=bb[j].iend)
				{
					indres=j;
					break;
				}
			}	
			areasum[indres]+=faces[i].area/3.0/scalefactor/scalefactor;
			for(j=0;j<numbb;j++)
			{
				if(verts[faces[i].c].atomid>=bb[j].istart
					&& verts[faces[i].c].atomid<=bb[j].iend)
				{
					indres=j;
					break;
				}
			}	
			areasum[indres]+=faces[i].area/3.0/scalefactor/scalefactor;
		}
	}
	double tval;
	FILE  *stream;
	stream =fopen(filename,"w+t");
	fprintf(stream,"indx res area   solve #chn #res\n");
	for(i=0;i<numbb;i++)
	{
		if(bb[i].indca!=-1 && proseq[bb[i].indca].simpletype==1)
		{
			tval=areasum[i]/asarea2[proseq[bb[i].indca].residueid];
			if(tval>1.0) tval=1.0;
			fprintf(stream,"%4d %c %8.3f %5.3f %c    %4d\n",
				i+1,bb[i].resid,areasum[i],tval,proseq[bb[i].indca].chainid,proseq[bb[i].indca].resno);
		}
		else
			fprintf(stderr,"lack ca atom %d/%d\n",i,numbb);
	}
	fclose(stream);

	delete[]areasum;
}

void ProteinSurface::outputcavityatoms(int seqinit,int seqterm,atom* proseq,char *filename)
{
	FILE  *stream;
    if( (stream =fopen(filename,"w+t"))== NULL)
	{
		printf("Wrong to write to file %s\n", filename);
		return;
	}
	int i,j;
	int ii,jj,kk;
	int iii,jjj,kkk;
	int si,sj,sk;
	bool *flagseq;
	flagseq=new bool[seqterm+1];
	for(j=0;j<ncav;j++)
	{
	//	fprintf(stream,"%2d:     volume--%8.4f\n",j,eachcavityvolume[j]);
		for(i=0;i<seqterm+1;i++)
		{
			flagseq[i]=false;
		}
		for(ii=0;ii<plength;ii++)
		{
			for(jj=0;jj<pwidth;jj++)
			{
				for(kk=0;kk<pheight;kk++)
				{
					if(!vp[ii][jj][kk].isdone && vp[ii][jj][kk].inout 
						&& int(vp[ii][jj][kk].distance)==j)
					{
						for(iii=-1;iii<2;iii++)
						{
							for(jjj=0;jjj<2;jjj++)
							{
								for(kkk=0;kkk<2;kkk++)
								{
									si=ii+iii;
									sj=jj+jjj;
									sk=kk+kkk;
									if(si>-1 && si<plength && 
										sj>-1 && sj<pwidth && 
										sk>-1 && sk<pheight && 
										vp[si][sj][sk].isdone &&
										!flagseq[vp[si][sj][sk].atomid])
									{
										flagseq[vp[si][sj][kk+kkk].atomid]=true;
									/*	fprintf(stream,"%2d:   %5d %c%c%c%c %c%c%c %c%4d    %8.3f%8.3f%8.3f %c%4d\n", j,
											proseq[vp[si][sj][sk].atomid].seqno,
											proseq[vp[si][sj][sk].atomid].detailtype[0],
											proseq[vp[si][sj][sk].atomid].detailtype[1],
											proseq[vp[si][sj][sk].atomid].detailtype[2],
											proseq[vp[si][sj][sk].atomid].detailtype[3],
											proseq[vp[si][sj][sk].atomid].residue[0],
											proseq[vp[si][sj][sk].atomid].residue[1],
											proseq[vp[si][sj][sk].atomid].residue[2],
											proseq[vp[si][sj][sk].atomid].chainid,
											proseq[vp[si][sj][sk].atomid].resno,
											proseq[vp[si][sj][sk].atomid].x,
											proseq[vp[si][sj][sk].atomid].y,
											proseq[vp[si][sj][sk].atomid].z,
											aad1[proseq[vp[si][sj][sk].atomid].residueid],
											proseq[vp[si][sj][sk].atomid].resno
											);*/
										fprintf(stream,"ATOM  %5d %c%c%c%c %c%c%c %c%4d    %8.3f%8.3f%8.3f %c %2d %8.3f\n", 
											proseq[vp[si][sj][sk].atomid].seqno,
											proseq[vp[si][sj][sk].atomid].detailtype[3],
											proseq[vp[si][sj][sk].atomid].detailtype[0],
											proseq[vp[si][sj][sk].atomid].detailtype[1],
											proseq[vp[si][sj][sk].atomid].detailtype[2],
											
											proseq[vp[si][sj][sk].atomid].residue[0],
											proseq[vp[si][sj][sk].atomid].residue[1],
											proseq[vp[si][sj][sk].atomid].residue[2],
											proseq[vp[si][sj][sk].atomid].chainid,
											proseq[vp[si][sj][sk].atomid].resno,
											proseq[vp[si][sj][sk].atomid].x,
											proseq[vp[si][sj][sk].atomid].y,
											proseq[vp[si][sj][sk].atomid].z,
											aad1[proseq[vp[si][sj][sk].atomid].residueid],j,eachcavityvolume[j]
											);
									}//if
								}
							}
						}
					}//if
				}
			}
		}//ii
	
	}//j
	delete[] flagseq;
	fclose(stream);
	
}
void ProteinSurface::boundingatom(bool btype)
{
	int i,j,k;
	double tradius[13];
	double txz,tdept,sradius;
	int indx;
	for(i=0;i<13;i++)
	{	
		if(depty[i]!=NULL)
			 free(depty[i]);
	}
	flagradius=btype;
	for(i=0;i<13;i++)
	{
		if(btype==false)
			tradius[i]=rasrad[i]*scalefactor+0.5;
		else 
			tradius[i]=(rasrad[i]+proberadius)*scalefactor+0.5;

		sradius=tradius[i]*tradius[i];
		widxz[i]=int(tradius[i])+1;
		depty[i]=new int[widxz[i]*widxz[i]];
		indx=0;
		for(j=0;j<widxz[i];j++)
		{
			for(k=0;k<widxz[i];k++)
			{
				txz=j*j+k*k;
				if(txz>sradius)
				{
					depty[i][indx]=-1;
				}
				else
				{
					tdept=sqrt(sradius-txz);
					depty[i][indx]=int(tdept+0.0); 
				}
				indx++;
			}
		}
		
	}

}
void ProteinSurface::initpara(int seqinit,int seqterm,atom* proseq,bool atomtype,bool btype)
{
	int i,j;
	double fmargin=2.5;
	if(vp!=NULL)
	{  
		for(i=0;i<plength;i++)
		{   
			for(j=0;j<pwidth;j++)
				free(vp[i][j]);
		}
		for(i=0;i<plength;i++)
			free(vp[i]);
		free(vp);
		vp=NULL;
	}
	boundbox(seqinit,seqterm,proseq,atomtype,&pmin,&pmax);
	if(btype==false)
	{
		pmin.x-=fmargin;
		pmin.y-=fmargin;
		pmin.z-=fmargin;
		pmax.x+=fmargin;
		pmax.y+=fmargin;
		pmax.z+=fmargin;
	}
	else
	{
		pmin.x-=proberadius+fmargin;
		pmin.y-=proberadius+fmargin;
		pmin.z-=proberadius+fmargin;
		pmax.x+=proberadius+fmargin;
		pmax.y+=proberadius+fmargin;
		pmax.z+=proberadius+fmargin;
	}

	ptran.x=-pmin.x;
	ptran.y=-pmin.y;
	ptran.z=-pmin.z;
	scalefactor=pmax.x-pmin.x;
	if((pmax.y-pmin.y)>scalefactor)
		scalefactor=pmax.y-pmin.y;
	if((pmax.z-pmin.z)>scalefactor)
		scalefactor=pmax.z-pmin.z;
	scalefactor=(boxlength-1.0)/double(scalefactor);
	///////////////////////////add this automatically first fix sf then fix boxlength
//	/*
	boxlength=int(boxlength*fixsf/scalefactor);
	scalefactor=fixsf;
	double threshbox=300;
	if(boxlength>threshbox)
	{
		double sfthresh=threshbox/double(boxlength);
		boxlength=int(threshbox);
		scalefactor=scalefactor*sfthresh;
	}
//	*/

	plength=int(ceil(scalefactor*(pmax.x-pmin.x))+1);
	pwidth=int(ceil(scalefactor*(pmax.y-pmin.y))+1);
	pheight=int(ceil(scalefactor*(pmax.z-pmin.z))+1);
	if(plength>boxlength)
		plength=boxlength;
	if(pwidth>boxlength)
		pwidth=boxlength;
	if(pheight>boxlength)
		pheight=boxlength;
	boundingatom(btype);
	cutradis=proberadius*scalefactor;
}
void ProteinSurface::fillatom(int indx,atom* proseq,bool bcolor)
{
	int cx,cy,cz;
	int ox,oy,oz;
	point3d cp;
	cp.x=proseq[indx].x+ptran.x;
	cp.y=proseq[indx].y+ptran.y;
	cp.z=proseq[indx].z+ptran.z;
	cp.x*=scalefactor;
	cp.y*=scalefactor;
	cp.z*=scalefactor;
	cx=int(cp.x+0.5);
	cy=int(cp.y+0.5);
	cz=int(cp.z+0.5);
	int at=proseq[indx].detail;
	int i,j,k;
	int ii,jj,kk;
	int mi,mj,mk;
	int si,sj,sk;
	int tind;
	int nind=0;
	for(i=0;i<widxz[at];i++)
	{
		for(j=0;j<widxz[at];j++)
		{
			if(depty[at][nind]!=-1)
			{
					
				for( ii=-1;ii<2;ii++)
				{
					for( jj=-1;jj<2;jj++)
					{
						for( kk=-1;kk<2;kk++)
						{
							if(ii!=0 && jj!=0 && kk!=0)
							{		
								mi=ii*i;
								mk=kk*j;
								for(k=0;k<=depty[at][nind];k++)
								{
									mj=k*jj;
									si=cx+mi;
									sj=cy+mj;
									sk=cz+mk;
									if(si<0 || sj<0 || sk<0 || si>=plength || sj>=pwidth || sk>=pheight)
									{
										continue;
									}
								if(!bcolor)
									{
										vp[si][sj][sk].inout=true;
										continue;
									}
								else{
									if(vp[si][sj][sk].inout==false)
									{
										vp[si][sj][sk].inout=true;
										vp[si][sj][sk].atomid=indx;
									}
									//no atomic info to each voxel change above line
								//*	
								    else if(vp[si][sj][sk].inout)
									{
										tind=vp[si][sj][sk].atomid;
										cp.x=proseq[tind].x+ptran.x;
										cp.y=proseq[tind].y+ptran.y;
										cp.z=proseq[tind].z+ptran.z;
										cp.x*=scalefactor;
										cp.y*=scalefactor;
										cp.z*=scalefactor;
										ox=int(cp.x+0.5)-si;
										oy=int(cp.y+0.5)-sj;
										oz=int(cp.z+0.5)-sk;
										if(mi*mi+mj*mj+mk*mk<ox*ox+oy*oy+oz*oz)
											vp[si][sj][sk].atomid=indx;
									}
								//	*/
								}//k
									}//else
							}//if
						}//kk	
					}//jj
				}//ii
			
				
			}//if
			nind++;
		}//j
	}//i
}
//sas use inout
void ProteinSurface::fillvoxels(int seqinit,int seqterm,bool atomtype,atom* proseq,bool bcolor)
{

	int i,j,k;
	if(vp==NULL)
	{
		vp=new volumepixel**[plength];
		for(i=0;i<plength;i++)
			vp[i]=new volumepixel*[pwidth];
		for(i=0;i<plength;i++)
		{   
			for(j=0;j<pwidth;j++)
			{
				vp[i][j]=new volumepixel[pheight];
			}
		}
	}
	
	for(i=0;i<plength;i++)
	{   
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vp[i][j][k].inout=false;
				vp[i][j][k].isdone=false;
				vp[i][j][k].isbound=false;
				vp[i][j][k].distance=-1;
				vp[i][j][k].atomid=-1;
			}
		}	
	}
//	int totnum=0;
	for(i=seqinit;i<=seqterm;i++)
	{
		if(proseq[i].simpletype==1 && proseq[i].ins==' ' /*&& (proseq[i].alt==' ' || proseq[i].alt=='A')*/ )
		{
			if(atomtype && (proseq[i].detail==5 || proseq[i].detail==12))
			{				
				continue;
			}
			fillatom(i,proseq,bcolor);
//			totnum++;
		}
	}
//	printf("%d\n",totnum);
	for(i=0;i<plength;i++)
	{   
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				if(vp[i][j][k].inout)
				{
					vp[i][j][k].isdone=true;
				}
			}
		}	
	}
}
//use isdone
void ProteinSurface::fillvoxelswaals(int seqinit,int seqterm,bool atomtype,atom* proseq,bool bcolor)
{
	int i,j,k;
	if(vp==NULL)
	{  
		vp=new volumepixel**[plength];
		for(i=0;i<plength;i++)
			vp[i]=new volumepixel*[pwidth];
		for(i=0;i<plength;i++)
		{   
			for(j=0;j<pwidth;j++)
			{
				vp[i][j]=new volumepixel[pheight];
			}
		}
	}
	

	for(i=0;i<plength;i++)
	{   
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vp[i][j][k].isdone=false;
			}
		}	
	}
	for(i=seqinit;i<=seqterm;i++)
	{
		if(proseq[i].simpletype==1 && proseq[i].ins==' ')
		{
			if(atomtype && (proseq[i].detail==5 ||proseq[i].detail==12))
			{
				continue;
			}
			fillatomwaals(i,proseq,bcolor);
		}
	}
}
void ProteinSurface::fillatomwaals(int indx,atom* proseq,bool bcolor)
{
	int cx,cy,cz;
	int ox,oy,oz;
	point3d cp;
	cp.x=proseq[indx].x+ptran.x;
	cp.y=proseq[indx].y+ptran.y;
	cp.z=proseq[indx].z+ptran.z;
	cp.x*=scalefactor;cp.y*=scalefactor;cp.z*=scalefactor;
	cx=int(cp.x+0.5);
	cy=int(cp.y+0.5);
	cz=int(cp.z+0.5);
	int at=proseq[indx].detail;
	int i,j,k;
	int ii,jj,kk;
	int mi,mj,mk;
	int si,sj,sk;
	int tind;
	int nind=0;
	for(i=0;i<widxz[at];i++)
	{
		for(j=0;j<widxz[at];j++)
		{
			if(depty[at][nind]!=-1)
			{
				
				for( ii=-1;ii<2;ii++)
				{
					for( jj=-1;jj<2;jj++)
					{
						for( kk=-1;kk<2;kk++)
						{
							if(ii!=0 && jj!=0 && kk!=0)
							{		
								mi=ii*i;
								mk=kk*j;
								for(k=0;k<=depty[at][nind];k++)
								{
									mj=k*jj;
									si=cx+mi;
									sj=cy+mj;
									sk=cz+mk;
									if(si<0 || sj<0 || sk<0)
									{
										continue;
									}
									if(!bcolor)
									{
										vp[si][sj][sk].isdone=true;
										continue;
									}
								else{
									if(vp[si][sj][sk].isdone==false)
									{
										vp[si][sj][sk].isdone=true;
										vp[si][sj][sk].atomid=indx;
									}
									//with atomic info change above line
								//*
								    else if(vp[si][sj][sk].isdone)
									{
										tind=vp[si][sj][sk].atomid;
										cp.x=proseq[tind].x+ptran.x;
										cp.y=proseq[tind].y+ptran.y;
										cp.z=proseq[tind].z+ptran.z;
										cp.x*=scalefactor;cp.y*=scalefactor;cp.z*=scalefactor;
										ox=int(cp.x+0.5)-si;
										oy=int(cp.y+0.5)-sj;
										oz=int(cp.z+0.5)-sk;
										if(mi*mi+mj*mj+mk*mk<ox*ox+oy*oy+oz*oz)
											vp[si][sj][sk].atomid=indx;
									}
							//	 */
									}//else
								}//k
								
							}//if
						}//kk	
					}//jj
				}//ii
				
				
			}//if
			nind++;
		}//j
	}//i
}

void ProteinSurface::fastdistancemap(int type)
{
	int i,j,k;
	int positin,positout,eliminate;
	int certificate;
	totalsurfacevox=0;
	totalinnervox=0;
	voxel2 ***boundpoint;
	boundpoint=new voxel2 **[plength];
	for(i=0;i<plength;i++)
	{
		boundpoint[i]=new voxel2*[pwidth];
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			boundpoint[i][j]=new voxel2[pheight];
		}
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vp[i][j][k].isdone=false;
				if(vp[i][j][k].inout)
				{
					if(vp[i][j][k].isbound)
					{
						totalsurfacevox++;
						boundpoint[i][j][k].ix=i;
						boundpoint[i][j][k].iy=j;
						boundpoint[i][j][k].iz=k;
						vp[i][j][k].distance=0;
						vp[i][j][k].isdone=true;
					}
				    else
					{
						totalinnervox++;
					}
				}
			}
		}
	}
	int allocin=int(1.2*totalsurfacevox);
	int allocout=int(1.2*totalsurfacevox);
	if(allocin>totalinnervox)
		allocin=totalinnervox;
	if(allocin<totalsurfacevox)
		allocin=totalsurfacevox;
	if(allocout>totalinnervox)
		allocout=totalinnervox;
	 inarray=new voxel2[allocin];
	 outarray=new voxel2[allocout];
	 positin=0;positout=0;
 
	 for(i=0;i<plength;i++)
	 {
		 for(j=0;j<pwidth;j++)
		 {
			 for(k=0;k<pheight;k++)
			 {	
				 if(vp[i][j][k].isbound)
				 {
					 inarray[positin].ix=i;
					 inarray[positin].iy=j;
					 inarray[positin].iz=k;
					 positin++;
					 vp[i][j][k].isbound=false;//as flag of outarray
				 }		 
			 }
		 }
	 }
	certificate=totalinnervox;
/////////////////////////////////////////////////// 
if(type==0)//do part
{
	do {
		fastoneshell(&positin, &allocout, boundpoint, &positout,&eliminate);
	//	printf("%d %d %d %d %d\n",positin,allocout,positout,totalsurfacevox,totalinnervox);
		certificate-=eliminate;
	/*
		for(i=0;i<positout;i++)
			{
			  inarray[i].ix=outarray[i].ix;
			  inarray[i].iy=outarray[i].iy;
			  inarray[i].iz=outarray[i].iz;
			}
			positin=positout;*/
		//new code only less dist
		positin=0;
		for(i=0;i<positout;i++)
		{
			vp[outarray[i].ix][outarray[i].iy][outarray[i].iz].isbound=false;
			if(vp[outarray[i].ix][outarray[i].iy][outarray[i].iz].distance<=1.02*cutradis)
			{
				inarray[positin].ix=outarray[i].ix;
				inarray[positin].iy=outarray[i].iy;
				inarray[positin].iz=outarray[i].iz;
				positin++;
			}
			if(positin>=allocin)
			{
				allocin*=2;
				if(allocin>totalinnervox) allocin=totalinnervox;
				inarray=(voxel2 *)realloc(inarray,allocin*sizeof(voxel2));
			}
		}
	} 
	while(positin!=0);
}
else if(type==1)//do all
{
	voxel2 *tpoint;
	do {
		
		fastoneshell( &positin, &allocout, boundpoint, &positout,&eliminate);//inarray, outarray,
		certificate-=eliminate;
//	/*
//			for(i=0;i<positout;i++)
//			{
//				vp[outarray[i].ix][outarray[i].iy][outarray[i].iz].isbound=false;
//			  inarray[i].ix=outarray[i].ix;
//			  inarray[i].iy=outarray[i].iy;
//			  inarray[i].iz=outarray[i].iz;
//			}
			tpoint=inarray;
			inarray=outarray;
			outarray=tpoint;
			positin=positout;
			int alloctmp;
			alloctmp=allocin;
			allocin=allocout;
			allocout=alloctmp;
			for(i=0;i<positin;i++)
				vp[inarray[i].ix][inarray[i].iy][inarray[i].iz].isbound=false;
//			*/
		//new code only less dist
		/*
		positin=0;
		for(i=0;i<positout;i++)
		{
			vp[outarray[i].ix][outarray[i].iy][outarray[i].iz].isbound=false;
			if(vp[outarray[i].ix][outarray[i].iy][outarray[i].iz].distance<=1.0*cutradis)
			{
				inarray[positin].ix=outarray[i].ix;
				inarray[positin].iy=outarray[i].iy;
				inarray[positin].iz=outarray[i].iz;
				positin++;
			}
		}
		*/
		
	} 
//	while(positin!=0);
	while(positout!=0);		
}
	//while(positout!=0);			 
	if(certificate!=0) 
	{
	//	printf("wrong number\n");
	}

	 free(inarray);
	 free(outarray);

	 double cutsf=scalefactor-0.5;
	 if(cutsf<0) cutsf=0;
//	 cutsf=100000000;
	 for(i=0;i<plength;i++)
	 {
		 for(j=0;j<pwidth;j++)
		 {
			 for(k=0;k<pheight;k++)
			 {
				 vp[i][j][k].isbound=false;
				 //ses solid
				 if(vp[i][j][k].inout)
				 {
					 if(!vp[i][j][k].isdone 
						 || (vp[i][j][k].isdone && vp[i][j][k].distance>=cutradis-0.50/(0.1+cutsf))//0.33  0.75/scalefactor
						 )
					 {
						 vp[i][j][k].isbound=true;
						 //new add
						 if(vp[i][j][k].isdone)
							vp[i][j][k].atomid=vp[boundpoint[i][j][k].ix][boundpoint[i][j][k].iy][boundpoint[i][j][k].iz].atomid;
					 }
				 }		 
			 }
		 }
	 }

	 
	 for(i=0;i<plength;i++)
	 {
		 for(j=0;j<pwidth;j++)
		 {
			 delete[]boundpoint[i][j];
		 }
	 }
	 
	 for(i=0;i<plength;i++)
	 {
		 delete[]boundpoint[i];
	 }
	 delete[] boundpoint;
}

void ProteinSurface::fastoneshell(int* innum,int *allocout,voxel2 ***boundpoint, int* outnum, int *elimi)
{
	int i, number,positout;
	int tx,ty,tz;
	int dx,dy,dz;
	int eliminate=0;
	float squre;
	positout=0;
	number=*innum;
	if(number==0) return;
	//new code
	int j;
	voxel tnv;
	for(i=0;i<number;i++)
	{
		if(positout>=(*allocout)-6)
		{
			(*allocout)=int(1.2*(*allocout));
			if(*allocout>totalinnervox) *allocout=totalinnervox;
			outarray=(voxel2 *)realloc(outarray,(*allocout)*sizeof(voxel2));
		}
		tx=inarray[i].ix;
        ty=inarray[i].iy;
		tz=inarray[i].iz;
		for(j=0;j<6;j++)
		{
			tnv.ix=tx+nb[j][0];
			tnv.iy=ty+nb[j][1];
			tnv.iz=tz+nb[j][2];
			if( tnv.ix<plength && tnv.ix>-1 && 
				tnv.iy<pwidth && tnv.iy>-1 && 
				tnv.iz<pheight && tnv.iz>-1 && 
				vp[tnv.ix][tnv.iy][tnv.iz].inout && 
				!vp[tnv.ix][tnv.iy][tnv.iz].isdone)
			{
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].ix=boundpoint[tx][ty][tz].ix;
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].iy=boundpoint[tx][ty][tz].iy;
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].iz=boundpoint[tx][ty][tz].iz;
				dx=tnv.ix-boundpoint[tx][ty][tz].ix;
				dy=tnv.iy-boundpoint[tx][ty][tz].iy;
				dz=tnv.iz-boundpoint[tx][ty][tz].iz;
				squre=float(dx*dx+dy*dy+dz*dz);
				vp[tnv.ix][tnv.iy][tnv.iz].distance=float(sqrt(squre));
				vp[tnv.ix][tnv.iy][tnv.iz].isdone=true;
				vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
				outarray[positout].ix=tnv.ix;
				outarray[positout].iy=tnv.iy;
				outarray[positout].iz=tnv.iz;
				positout++;eliminate++;
			}
			else if( tnv.ix<plength && tnv.ix>-1 && 
				tnv.iy<pwidth && tnv.iy>-1 && 
				tnv.iz<pheight && tnv.iz>-1 && 
				vp[tnv.ix][tnv.iy][tnv.iz].inout && 
				vp[tnv.ix][tnv.iy][tnv.iz].isdone)
			{
				
				dx=tnv.ix-boundpoint[tx][ty][tz].ix;
				dy=tnv.iy-boundpoint[tx][ty][tz].iy;
				dz=tnv.iz-boundpoint[tx][ty][tz].iz;
				squre=float(dx*dx+dy*dy+dz*dz);
				squre=float(sqrt(squre));
				if(squre<vp[tnv.ix][tnv.iy][tnv.iz].distance)
				{
					boundpoint[tnv.ix][tnv.iy][tnv.iz].ix=boundpoint[tx][ty][tz].ix;
					boundpoint[tnv.ix][tnv.iy][tnv.iz].iy=boundpoint[tx][ty][tz].iy;
					boundpoint[tnv.ix][tnv.iy][tnv.iz].iz=boundpoint[tx][ty][tz].iz;
					vp[tnv.ix][tnv.iy][tnv.iz].distance=float(squre);
					if(!vp[tnv.ix][tnv.iy][tnv.iz].isbound)
					{
						vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
						outarray[positout].ix=tnv.ix;
						outarray[positout].iy=tnv.iy;
						outarray[positout].iz=tnv.iz;
						positout++;
					}
				}
		
			}
		}
	}
	for(i=0;i<number;i++)
	{
		if(positout>=(*allocout)-12)
		{
			(*allocout)=int(1.2*(*allocout));
			if(*allocout>totalinnervox) *allocout=totalinnervox;
			outarray=(voxel2 *)realloc(outarray,(*allocout)*sizeof(voxel2));
		}
		tx=inarray[i].ix;
        ty=inarray[i].iy;
		tz=inarray[i].iz;
		for(j=6;j<18;j++)
		{
			tnv.ix=tx+nb[j][0];
			tnv.iy=ty+nb[j][1];
			tnv.iz=tz+nb[j][2];
			if( tnv.ix<plength && tnv.ix>-1 && 
				tnv.iy<pwidth && tnv.iy>-1 && 
				tnv.iz<pheight && tnv.iz>-1 && 
				vp[tnv.ix][tnv.iy][tnv.iz].inout && 
				!vp[tnv.ix][tnv.iy][tnv.iz].isdone)
			{
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].ix=boundpoint[tx][ty][tz].ix;
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].iy=boundpoint[tx][ty][tz].iy;
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].iz=boundpoint[tx][ty][tz].iz;
				dx=tnv.ix-boundpoint[tx][ty][tz].ix;
				dy=tnv.iy-boundpoint[tx][ty][tz].iy;
				dz=tnv.iz-boundpoint[tx][ty][tz].iz;
				squre=float(dx*dx+dy*dy+dz*dz);
				vp[tnv.ix][tnv.iy][tnv.iz].distance=float(sqrt(squre));
				vp[tnv.ix][tnv.iy][tnv.iz].isdone=true;
				vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
				outarray[positout].ix=tnv.ix;
				outarray[positout].iy=tnv.iy;
				outarray[positout].iz=tnv.iz;
				positout++;eliminate++;
			}
			else if( tnv.ix<plength && tnv.ix>-1 && 
				tnv.iy<pwidth && tnv.iy>-1 && 
				tnv.iz<pheight && tnv.iz>-1 && 
				vp[tnv.ix][tnv.iy][tnv.iz].inout && 
				vp[tnv.ix][tnv.iy][tnv.iz].isdone)
			{
				dx=tnv.ix-boundpoint[tx][ty][tz].ix;
				dy=tnv.iy-boundpoint[tx][ty][tz].iy;
				dz=tnv.iz-boundpoint[tx][ty][tz].iz;
				squre=float(dx*dx+dy*dy+dz*dz);
				squre=float(sqrt(squre));
				if(squre<vp[tnv.ix][tnv.iy][tnv.iz].distance)
				{
					boundpoint[tnv.ix][tnv.iy][tnv.iz].ix=boundpoint[tx][ty][tz].ix;
					boundpoint[tnv.ix][tnv.iy][tnv.iz].iy=boundpoint[tx][ty][tz].iy;
					boundpoint[tnv.ix][tnv.iy][tnv.iz].iz=boundpoint[tx][ty][tz].iz;
					vp[tnv.ix][tnv.iy][tnv.iz].distance=float(squre);
					if(!vp[tnv.ix][tnv.iy][tnv.iz].isbound)
					{
						vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
						outarray[positout].ix=tnv.ix;
						outarray[positout].iy=tnv.iy;
						outarray[positout].iz=tnv.iz;
						positout++;
					}
				}
				
			}
		}
	}
	for(i=0;i<number;i++)
	{
		if(positout>=(*allocout)-9)
		{
			(*allocout)=int(1.2*(*allocout));
			if(*allocout>totalinnervox) *allocout=totalinnervox;
			outarray=(voxel2 *)realloc(outarray,(*allocout)*sizeof(voxel2));
		}
		tx=inarray[i].ix;
        ty=inarray[i].iy;
		tz=inarray[i].iz;
		for(j=18;j<26;j++)
		{
			tnv.ix=tx+nb[j][0];
			tnv.iy=ty+nb[j][1];
			tnv.iz=tz+nb[j][2];
			if( tnv.ix<plength && tnv.ix>-1 && 
				tnv.iy<pwidth && tnv.iy>-1 && 
				tnv.iz<pheight && tnv.iz>-1 && 
				vp[tnv.ix][tnv.iy][tnv.iz].inout && 
				!vp[tnv.ix][tnv.iy][tnv.iz].isdone)
			{
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].ix=boundpoint[tx][ty][tz].ix;
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].iy=boundpoint[tx][ty][tz].iy;
				boundpoint[tnv.ix][tnv.iy][tz+nb[j][2]].iz=boundpoint[tx][ty][tz].iz;
				dx=tnv.ix-boundpoint[tx][ty][tz].ix;
				dy=tnv.iy-boundpoint[tx][ty][tz].iy;
				dz=tnv.iz-boundpoint[tx][ty][tz].iz;
				squre=float(dx*dx+dy*dy+dz*dz);
				vp[tnv.ix][tnv.iy][tnv.iz].distance=float(sqrt(squre));
				vp[tnv.ix][tnv.iy][tnv.iz].isdone=true;
				vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
				outarray[positout].ix=tnv.ix;
				outarray[positout].iy=tnv.iy;
				outarray[positout].iz=tnv.iz;
				positout++;eliminate++;
			}
			else if( tnv.ix<plength && tnv.ix>-1 && 
				tnv.iy<pwidth && tnv.iy>-1 && 
				tnv.iz<pheight && tnv.iz>-1 && 
				vp[tnv.ix][tnv.iy][tnv.iz].inout && 
				vp[tnv.ix][tnv.iy][tnv.iz].isdone)
			{
				
				dx=tnv.ix-boundpoint[tx][ty][tz].ix;
				dy=tnv.iy-boundpoint[tx][ty][tz].iy;
				dz=tnv.iz-boundpoint[tx][ty][tz].iz;
				squre=float(dx*dx+dy*dy+dz*dz);
				squre=float(sqrt(squre));
				if(squre<vp[tnv.ix][tnv.iy][tnv.iz].distance)
				{
					boundpoint[tnv.ix][tnv.iy][tnv.iz].ix=boundpoint[tx][ty][tz].ix;
					boundpoint[tnv.ix][tnv.iy][tnv.iz].iy=boundpoint[tx][ty][tz].iy;
					boundpoint[tnv.ix][tnv.iy][tnv.iz].iz=boundpoint[tx][ty][tz].iz;
					vp[tnv.ix][tnv.iy][tnv.iz].distance=float(squre);
					if(!vp[tnv.ix][tnv.iy][tnv.iz].isbound)
					{
						vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
						outarray[positout].ix=tnv.ix;
						outarray[positout].iy=tnv.iy;
						outarray[positout].iz=tnv.iz;
						positout++;
					}
				}
				
			}
		}
	}
	
	*outnum=positout;
	*elimi=eliminate;
	 
}
//include inner
void ProteinSurface::calcareavolume()
{
	int i,j,k;
	double totvol=0.10*scalefactor*vertnumber;
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				if(vp[i][j][k].isdone)
					totvol+=1;			
			}
		}
	}
	double totarea=0;
	for(i=0;i<facenumber;i++)
	{
		totarea+=faces[i].area;
	}

	sarea=totarea/scalefactor/scalefactor;
	svolume=(totvol)/scalefactor/scalefactor/scalefactor;

}
//cavity number
void ProteinSurface::cavitynumbers()
{
	int i,j,k;
	int l,ii,jj,kk;
	voxel *inarray,*outarray;
	voxel *tarray,tnv;
	int innum=0;
	int outnum;
	int totnum=0;
	if(eachcavityvolume!=NULL)
	{
		free(eachcavityvolume);
	}
	eachcavityvolume=new double[1500];
	for(i=0;i<1500;i++)
	{
		eachcavityvolume[i]=0;
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vp[i][j][k].isbound=false;
				if(!vp[i][j][k].isdone)
				{
					totnum++;
				}
			}
		}
	}
	inarray=new voxel[totnum];
	outarray=new voxel[totnum];
	ncav=0;
	for(l=0;l<plength;l++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				if(!vp[l][j][k].isbound && !vp[l][j][k].isdone && vp[l][j][k].inout)
				{
					vp[l][j][k].distance=float(ncav);
					vp[l][j][k].isbound=true;
					innum=0;
					inarray[innum].ix=l;
					inarray[innum].iy=j;
					inarray[innum].iz=k;
					innum++;
					eachcavityvolume[ncav]=eachcavityvolume[ncav]+1;
					while(innum!=0)
					{
						outnum=0;
						for(i=0;i<innum;i++)
						{
							for(ii=-1;ii<2;ii++)
							{
								for(jj=-1;jj<2;jj++)
								{
									for(kk=-1;kk<2;kk++)
									{
										tnv.ix=inarray[i].ix+ii;
										tnv.iy=inarray[i].iy+jj;
										tnv.iz=inarray[i].iz+kk;
										if( abs(ii)+abs(jj)+abs(kk)<2 &&
											tnv.ix>-1 && tnv.ix<plength 
											&& tnv.iy>-1 && tnv.iy<pwidth
											&& tnv.iz>-1 && tnv.iz<pheight
											&& vp[tnv.ix][tnv.iy][tnv.iz].inout
											&& !vp[tnv.ix][tnv.iy][tnv.iz].isdone
											&& !vp[tnv.ix][tnv.iy][tnv.iz].isbound)
										{
											vp[tnv.ix][tnv.iy][tnv.iz].isbound=true;
											vp[tnv.ix][tnv.iy][tnv.iz].distance=float(ncav);
											outarray[outnum].ix=tnv.ix;
											outarray[outnum].iy=tnv.iy;
											outarray[outnum].iz=tnv.iz;
											outnum++;
											eachcavityvolume[ncav]=eachcavityvolume[ncav]+1;
										}
									}//kk
								}//jj
							}//ii
						}//i
						tarray=outarray;
						outarray=inarray;
						inarray=tarray;
						innum=outnum;
					}//while
					ncav++;
				}//if
			}//k
		}//j
	}//l
	delete[]inarray;
	delete[]outarray;
	for(i=0;i<ncav;i++)
	{
		eachcavityvolume[i]=eachcavityvolume[i]/scalefactor/scalefactor/scalefactor;
	}

}
//cavity area and volume surface inner or outer
void ProteinSurface::cavitiesareavolume()
{
	int i,j,k;
	double totarea=0;
	int totvol=0;
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				if(!vp[i][j][k].isdone && vp[i][j][k].inout)
					totvol++;
			}
		}
	}
	int tx[3],ty[3],tz[3];
	int ii,jj,kk;
	bool flaginner;
	for(i=0;i<facenumber;i++)
	{
		faces[i].inout=false;
		verts[faces[i].a].inout=true;
		verts[faces[i].b].inout=true;
		verts[faces[i].c].inout=true;
		flaginner=false;
		tx[0]=int(verts[faces[i].a].x);
		ty[0]=int(verts[faces[i].a].y);
		tz[0]=int(verts[faces[i].a].z);
		tx[1]=int(verts[faces[i].b].x);
		ty[1]=int(verts[faces[i].b].y);
		tz[1]=int(verts[faces[i].b].z);
		tx[2]=int(verts[faces[i].c].x);
		ty[2]=int(verts[faces[i].c].y);
		tz[2]=int(verts[faces[i].c].z);
		for(ii=-1;ii<2;ii++)
		{
			for(jj=-1;jj<2;jj++)
			{
				for(kk=-1;kk<2;kk++)
				{
					if(!(ii==0 && jj==0 && kk==0) && tx[0]+ii>-1 &&  tx[0]+ii<plength
						&& ty[0]+jj>-1 &&  ty[0]+jj<pwidth
						&& tz[0]+kk>-1 &&  tz[0]+kk<pheight
						&& vp[tx[0]+ii][ty[0]+jj][tz[0]+kk].inout
						&& !vp[tx[0]+ii][ty[0]+jj][tz[0]+kk].isdone)
					{
						flaginner=true;
					}
				}
			}
		}
		for(ii=-1;ii<2;ii++)
		{
			for(jj=-1;jj<2;jj++)
			{
				for(kk=-1;kk<2;kk++)
				{
					if(!(ii==0 && jj==0 && kk==0) && tx[1]+ii>-1 &&  tx[1]+ii<plength
						&& ty[1]+jj>-1 &&  ty[1]+jj<pwidth
						&& tz[1]+kk>-1 &&  tz[1]+kk<pheight
						&& vp[tx[1]+ii][ty[1]+jj][tz[1]+kk].inout
						&& !vp[tx[1]+ii][ty[1]+jj][tz[1]+kk].isdone)
					{
						flaginner=true;
					}
				}
			}
		}
		for(ii=-1;ii<2;ii++)
		{
			for(jj=-1;jj<2;jj++)
			{
				for(kk=-1;kk<2;kk++)
				{
					if(!(ii==0 && jj==0 && kk==0) && tx[2]+ii>-1 &&  tx[2]+ii<plength
						&& ty[2]+jj>-1 &&  ty[2]+jj<pwidth
						&& tz[2]+kk>-1 &&  tz[2]+kk<pheight
						&& vp[tx[2]+ii][ty[2]+jj][tz[2]+kk].inout
						&& !vp[tx[2]+ii][ty[2]+jj][tz[2]+kk].isdone)
					{
						flaginner=true;
					}
				}
			}
		}
		if(flaginner)
		{
			faces[i].inout=true;
			verts[faces[i].a].inout=false;
			verts[faces[i].b].inout=false;
			verts[faces[i].c].inout=false;
			totarea+=faces[i].area;
		}
	}
	carea=totarea/scalefactor/scalefactor;
	cvolume=totvol/scalefactor/scalefactor/scalefactor;
}
void ProteinSurface::computenorm()
{
	int i;
	double pnorm;
	for(i=0;i<vertnumber;i++)
	{
		verts[i].pn.x=0;
		verts[i].pn.y=0;
		verts[i].pn.z=0;
	}
	point3d p1,p2,p3;
	point3d p12,p13;
	point3d pn;
	for(i=0;i<facenumber;i++)
	{
		p1.x=verts[faces[i].a].x;
		p1.y=verts[faces[i].a].y;
		p1.z=verts[faces[i].a].z;
		p2.x=verts[faces[i].b].x;
		p2.y=verts[faces[i].b].y;
		p2.z=verts[faces[i].b].z;
		p3.x=verts[faces[i].c].x;
		p3.y=verts[faces[i].c].y;
		p3.z=verts[faces[i].c].z;
		p12.x=p2.x-p1.x;
		p12.y=p2.y-p1.y;
		p12.z=p2.z-p1.z;
		p13.x=p3.x-p1.x;
		p13.y=p3.y-p1.y;
		p13.z=p3.z-p1.z;
		pn.x=p12.y*p13.z-p12.z*p13.y;
		pn.y=p12.z*p13.x-p12.x*p13.z;
		pn.z=p12.x*p13.y-p12.y*p13.x;
		faces[i].area=0.5*sqrt(pn.x*pn.x+pn.y*pn.y+pn.z*pn.z);
		faces[i].pn.x=0.5*pn.x/faces[i].area;
		faces[i].pn.y=0.5*pn.y/faces[i].area;
		faces[i].pn.z=0.5*pn.z/faces[i].area;
		//*
		//without area
		verts[faces[i].a].pn.x+=faces[i].pn.x;
		verts[faces[i].a].pn.y+=faces[i].pn.y;
		verts[faces[i].a].pn.z+=faces[i].pn.z;
		verts[faces[i].b].pn.x+=faces[i].pn.x;
		verts[faces[i].b].pn.y+=faces[i].pn.y;
		verts[faces[i].b].pn.z+=faces[i].pn.z;
		verts[faces[i].c].pn.x+=faces[i].pn.x;
		verts[faces[i].c].pn.y+=faces[i].pn.y;
		verts[faces[i].c].pn.z+=faces[i].pn.z;
		//*/
		/*
		//with area
		verts[faces[i].a].pn.x+=pn.x;
		verts[faces[i].a].pn.y+=pn.y;
		verts[faces[i].a].pn.z+=pn.z;
		verts[faces[i].b].pn.x+=pn.x;
		verts[faces[i].b].pn.y+=pn.y;
		verts[faces[i].b].pn.z+=pn.z;
		verts[faces[i].c].pn.x+=pn.x;
		verts[faces[i].c].pn.y+=pn.y;
		verts[faces[i].c].pn.z+=pn.z;
		*/
	}
	for(i=0;i<vertnumber;i++)
	{
		pn.x=verts[i].pn.x;
		pn.y=verts[i].pn.y;
		pn.z=verts[i].pn.z;
		pnorm=sqrt(pn.x*pn.x+pn.y*pn.y+pn.z*pn.z);
		if(pnorm==0.0)
		{
			pn.x=0.0;
			pn.y=0.0;
			pn.z=0.0;
			verts[i].pn.x=pn.x;
			verts[i].pn.y=pn.y;
			verts[i].pn.z=pn.z;
			continue;
		}
		pn.x/=pnorm;
		pn.y/=pnorm;
		pn.z/=pnorm;
		verts[i].pn.x=pn.x;
		verts[i].pn.y=pn.y;
		verts[i].pn.z=pn.z;
	}
}
void ProteinSurface::marchingcubeinit(int stype)
{
	int i,j,k;
	//vdw
	if(stype==1)
	{
		for(i=0;i<plength;i++)
		{
			for(j=0;j<pwidth;j++)
			{
				for(k=0;k<pheight;k++)
				{
					vp[i][j][k].isbound=false;
				}
			}
		}

	}
	//ses
	else if(stype==4)
	{
		///////////////without vdw
		for(i=0;i<plength;i++)
		{
			for(j=0;j<pwidth;j++)
			{
				for(k=0;k<pheight;k++)
				{
					vp[i][j][k].isdone=false;
					if(vp[i][j][k].isbound)
					{
						vp[i][j][k].isdone=true;	
					}
					//new add
					vp[i][j][k].isbound=false;
				}
			}
		}
		
	}
	else if(stype==2)
	{	
		///////////////////////after vdw
		for(i=0;i<plength;i++)
		{
			for(j=0;j<pwidth;j++)
			{
				for(k=0;k<pheight;k++)
				{
				//	if(vp[i][j][k].inout && vp[i][j][k].distance>=cutradis)
					if(vp[i][j][k].isbound && vp[i][j][k].isdone)
					{
						vp[i][j][k].isbound=false;
					}
					else if(vp[i][j][k].isbound && !vp[i][j][k].isdone)
					{
						vp[i][j][k].isdone=true;	
					}
				}
			}
		}
		
	}
	//sas
	else if(stype==3)
	{
		for(i=0;i<plength;i++)
		{
			for(j=0;j<pwidth;j++)
			{
				for(k=0;k<pheight;k++)
				{
					vp[i][j][k].isbound=false;
				}
			}
		}
	}
	
}
//half accuracy
void ProteinSurface::marchingcubeorigin(int stype)
{
	int i,j,k;
	marchingcubeinit(stype);
	int ***vertseq;
	vertseq=new int**[plength];
	for(i=0;i<plength;i++)
	{
		vertseq[i]=new int*[pwidth];
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			vertseq[i][j]=new int[pheight];
		}
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vertseq[i][j][k]=-1;
			}		
		}
	}
	if(faces!=NULL)
	{
		free(faces);
	}
	if(verts!=NULL)
	{
		free(verts);
	}
	int allocface=20;
	int allocvert=12;
	facenumber=0;
	vertnumber=0;
	verts=new vertinfo[allocvert];
	faces=new faceinfo[allocface];
	///////////////////////////////////////////
	int ii,jj;
	int tl[3];
	voxel tp[3][2],tv[3];
	int totind=0;
	for(i=0;i<plength-2;i+=2)
	{
		for(j=0;j<pwidth-2;j+=2)
		{
			for(k=0;k<pheight-2;k+=2)
			{
				if(vertnumber+8>allocvert)
				{
					allocvert*=2;
					verts=(vertinfo *)realloc(verts,allocvert*sizeof(vertinfo));
				}
				if(facenumber+5>allocface)
				{
					allocface*=2;
					faces=(faceinfo *)realloc(faces,allocface*sizeof(faceinfo));
				}
				totind=0;
				for(ii=0;ii<8;ii++)
				{
					if(!vp[i+2*a2fVertexOffset[ii][0]][j+2*a2fVertexOffset[ii][1]][k+2*a2fVertexOffset[ii][2]].isdone)
						totind |= 1<<ii;	
				}
				for(ii = 0; ii < 5; ii++)
				{
					if(a2iTriangleConnectionTable[totind][3*ii] < 0)
                        break;
					for(jj=0;jj<3;jj++)
					{
						tl[jj]=a2iTriangleConnectionTable[totind][3*ii+jj];
						tp[jj][0].ix=i+2*a2fVertexOffset[a2iEdgeConnection[tl[jj]][0]][0];
						tp[jj][0].iy=j+2*a2fVertexOffset[a2iEdgeConnection[tl[jj]][0]][1];
						tp[jj][0].iz=k+2*a2fVertexOffset[a2iEdgeConnection[tl[jj]][0]][2];
						tp[jj][1].ix=i+2*a2fVertexOffset[a2iEdgeConnection[tl[jj]][1]][0];
						tp[jj][1].iy=j+2*a2fVertexOffset[a2iEdgeConnection[tl[jj]][1]][1];
						tp[jj][1].iz=k+2*a2fVertexOffset[a2iEdgeConnection[tl[jj]][1]][2];
						tv[jj].ix=int(tp[jj][0].ix+tp[jj][1].ix)/2;
						tv[jj].iy=int(tp[jj][0].iy+tp[jj][1].iy)/2;
						tv[jj].iz=int(tp[jj][0].iz+tp[jj][1].iz)/2;
						if(!vp[tv[jj].ix][tv[jj].iy][tv[jj].iz].isdone)
						{
							if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isdone)
								vp[tv[jj].ix][tv[jj].iy][tv[jj].iz]=vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz];
							else if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isdone)
								vp[tv[jj].ix][tv[jj].iy][tv[jj].iz]=vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz];
							

						}
						if(vertseq[tv[jj].ix][tv[jj].iy][tv[jj].iz]==-1)
						{
							vertseq[tv[jj].ix][tv[jj].iy][tv[jj].iz]=vertnumber;
							verts[vertnumber].x=tv[jj].ix;
							verts[vertnumber].y=tv[jj].iy;
							verts[vertnumber].z=tv[jj].iz;
							vertnumber++;
						}
					}	
				faces[facenumber].a=vertseq[tv[0].ix][tv[0].iy][tv[0].iz];
				faces[facenumber].b=vertseq[tv[1].ix][tv[1].iy][tv[1].iz];
				faces[facenumber++].c=vertseq[tv[2].ix][tv[2].iy][tv[2].iz];
				}
			}
		}
	}

	verts=(vertinfo *)realloc(verts,vertnumber*sizeof(vertinfo));
	faces=(faceinfo *)realloc(faces,facenumber*sizeof(faceinfo));
	
	for(i=0;i<vertnumber;i++)
	{
		verts[i].atomid=vp[int(verts[i].x)][int(verts[i].y)][int(verts[i].z)].atomid;
		verts[i].iscont=false;
		if(vp[int(verts[i].x)][int(verts[i].y)][int(verts[i].z)].isbound)
			verts[i].iscont=true;
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			delete[]vertseq[i][j];
		}
	}
	
	for(i=0;i<plength;i++)
	{
		delete[]vertseq[i];
	}
	delete[]vertseq;

}
// middle points
void ProteinSurface::marchingcubeorigin2(int stype)
{
	int i,j,k;
	voxel ***vpind;
	vpind=new voxel**[plength];
	for(i=0;i<plength;i++)
	{
		vpind[i]=new voxel*[pwidth];
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			vpind[i][j]=new voxel[pheight];
		}
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vpind[i][j][k].ix=-1;
				vpind[i][j][k].iy=-1;
				vpind[i][j][k].iz=-1;
			}
		}
	}
	marchingcubeinit(stype);
	if(faces!=NULL)
	{
		free(faces);
	}
	if(verts!=NULL)
	{
		free(verts);
	}
	int allocface=20;
	int allocvert=12;
	facenumber=0;
	vertnumber=0;
	verts=new vertinfo[allocvert];
	faces=new faceinfo[allocface];



	///////////////////////////////////////////
	int ii,jj;
	int tl[3];
	voxel tp[3][2];
	point3d tv[3];
	int indv[3];
	int totind=0;
	int curpt;
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vertnumber+8>allocvert)
				{
					allocvert*=2;
					verts=(vertinfo *)realloc(verts,allocvert*sizeof(vertinfo));
				}
				if(facenumber+5>allocface)
				{
					allocface*=2;
					faces=(faceinfo *)realloc(faces,allocface*sizeof(faceinfo));
				}
				totind=0;
				for(ii=0;ii<8;ii++)
				{
					if(!vp[i+a2fVertexOffset[ii][0]][j+a2fVertexOffset[ii][1]][k+a2fVertexOffset[ii][2]].isdone)
						totind |= 1<<ii;	
				}
				for(ii = 0; ii < 5; ii++)
				{
					if(a2iTriangleConnectionTable[totind][3*ii] < 0)
                        break;
					for(jj=0;jj<3;jj++)
					{
						tl[jj]=a2iTriangleConnectionTable[totind][3*ii+jj];
						tp[jj][0].ix=i+a2fVertexOffset[a2iEdgeConnection[tl[jj]][0]][0];
						tp[jj][0].iy=j+a2fVertexOffset[a2iEdgeConnection[tl[jj]][0]][1];
						tp[jj][0].iz=k+a2fVertexOffset[a2iEdgeConnection[tl[jj]][0]][2];
						tp[jj][1].ix=i+a2fVertexOffset[a2iEdgeConnection[tl[jj]][1]][0];
						tp[jj][1].iy=j+a2fVertexOffset[a2iEdgeConnection[tl[jj]][1]][1];
						tp[jj][1].iz=k+a2fVertexOffset[a2iEdgeConnection[tl[jj]][1]][2];
						tv[jj].x=(tp[jj][0].ix+tp[jj][1].ix)/2.0;
						tv[jj].y=(tp[jj][0].iy+tp[jj][1].iy)/2.0;
						tv[jj].z=(tp[jj][0].iz+tp[jj][1].iz)/2.0;
												
						if(tp[jj][0].ix!=tp[jj][1].ix)
						{
							curpt=int(tv[jj].x);
							if(vpind[curpt][tp[jj][1].iy][tp[jj][1].iz].ix==-1)
							{
								indv[jj]=vertnumber;
								vpind[curpt][tp[jj][1].iy][tp[jj][1].iz].ix=vertnumber;
								verts[vertnumber].x=tv[jj].x;
								verts[vertnumber].y=tv[jj].y;
								verts[vertnumber].z=tv[jj].z;
								if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isdone)
								{
									verts[vertnumber].atomid=vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].atomid;
									verts[vertnumber].iscont=false;
									if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isbound)
										verts[vertnumber].iscont=true;
								}
								else if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isdone)
								{
									verts[vertnumber].atomid=vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].atomid;
									verts[vertnumber].iscont=false;
									if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isbound)
										verts[vertnumber].iscont=true;
								}
								
								vertnumber++;
							}
							else
							{
								indv[jj]=vpind[curpt][tp[jj][1].iy][tp[jj][1].iz].ix;
							}
						}	
						else if(tp[jj][0].iy!=tp[jj][1].iy)
						{
							curpt=int(tv[jj].y);
							if(vpind[tp[jj][1].ix][curpt][tp[jj][1].iz].iy==-1)
							{
								indv[jj]=vertnumber;
								vpind[tp[jj][1].ix][curpt][tp[jj][1].iz].iy=vertnumber;
								verts[vertnumber].x=tv[jj].x;
								verts[vertnumber].y=tv[jj].y;
								verts[vertnumber].z=tv[jj].z;
								if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isdone)
								{
									verts[vertnumber].atomid=vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].atomid;
									verts[vertnumber].iscont=false;
									if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isbound)
										verts[vertnumber].iscont=true;
								}
								else if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isdone)
								{
									verts[vertnumber].atomid=vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].atomid;
									verts[vertnumber].iscont=false;
									if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isbound)
										verts[vertnumber].iscont=true;
								}
								
								vertnumber++;
							}
							else
							{
								indv[jj]=vpind[tp[jj][1].ix][curpt][tp[jj][1].iz].iy;
							}
						}
						else if(tp[jj][0].iz!=tp[jj][1].iz)
						{
							curpt=int(tv[jj].z);
							if(vpind[tp[jj][1].ix][tp[jj][1].iy][curpt].iz==-1)
							{
								indv[jj]=vertnumber;
								vpind[tp[jj][1].ix][tp[jj][1].iy][curpt].iz=vertnumber;
								verts[vertnumber].x=tv[jj].x;
								verts[vertnumber].y=tv[jj].y;
								verts[vertnumber].z=tv[jj].z;
								if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isdone)
								{
									verts[vertnumber].atomid=vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].atomid;
									verts[vertnumber].iscont=false;
									if(vp[tp[jj][0].ix][tp[jj][0].iy][tp[jj][0].iz].isbound)
										verts[vertnumber].iscont=true;
								}
								else if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isdone)
								{
									verts[vertnumber].atomid=vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].atomid;
									verts[vertnumber].iscont=false;
									if(vp[tp[jj][1].ix][tp[jj][1].iy][tp[jj][1].iz].isbound)
										verts[vertnumber].iscont=true;
								}
								
								vertnumber++;
							}
							else
							{
								indv[jj]=vpind[tp[jj][1].ix][tp[jj][1].iy][curpt].iz;
							}
						}	
					}//jj	
					faces[facenumber].a=indv[0];
					faces[facenumber].b=indv[1];
					faces[facenumber++].c=indv[2];
				}//ii
			}//k
		}//j
	}//i
	
	verts=(vertinfo *)realloc(verts,vertnumber*sizeof(vertinfo));
	faces=(faceinfo *)realloc(faces,facenumber*sizeof(faceinfo));
	
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			delete[] vpind[i][j];
		}
	}
	for(i=0;i<plength;i++)
	{
		delete[] vpind[i];
	}
	delete[] vpind;

}
/*
void ProteinSurface::marchingcube(int stype)
{
	int i,j,k;
	marchingcubeinit(stype);
	int ***vertseq;
	vertseq=new int**[plength];
	for(i=0;i<plength;i++)
	{
		vertseq[i]=new int*[pwidth];
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			vertseq[i][j]=new int[pheight];
		}
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vertseq[i][j][k]=-1;
			}		
		}
	}
	if(faces!=NULL)
	{
		free(faces);
	}
	if(verts!=NULL)
	{
		free(verts);
	}
	int allocvert=4*(pheight*plength+pwidth*plength+pheight*pwidth);
	int allocface=2*allocvert;
	
	facenumber=0;
	vertnumber=0;
	verts=new vertinfo[allocvert];
	faces=new faceinfo[allocface];

	int ii,jj,kk;
	int tp[6][3];
	/////////////////////////////////////////new added  normal is outer
	//face1
	for(i=0;i<1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
					tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
				    tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].b=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					||( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					}
				    else if( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face3
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face5
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<1;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone
					&& vp[i][j+1][k].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].b=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					||( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					||(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face2
	for(i=plength-1;i<plength;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
					tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					||( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					}
					else if( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face4
	for(i=0;i<plength-1;i++)
	{
		for(j=pwidth-1;j<pwidth;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].b=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face6
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=pheight-1;k<pheight;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone
					&& vp[i][j+1][k].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					||( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					||(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	////////////////////////////////////////////vcmc
	int indtype;
	char binout[8];
	int coord[8][3];
	int ptind[3];
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				jj=1;
				indtype=0;
				for(ii=0;ii<8;ii++)
				{
					coord[ii][0]=i+vcmcoffset[ii][0];
					coord[ii][1]=j+vcmcoffset[ii][1];
					coord[ii][2]=k+vcmcoffset[ii][2];
					if(vp[coord[ii][0]][coord[ii][1]][coord[ii][2]].isdone)
						binout[ii]=1;
					else binout[ii]=0;
					indtype+=binout[ii]*jj;
					jj*=2;
				}
				jj=1;
				for(ii=0;ii<vcmctable[indtype][0];ii++)
				{	
					for(kk=0;kk<3;kk++)
					{
						ptind[kk]=vcmctable[indtype][jj+kk]; 
						if(vertseq[coord[ptind[kk]][0]][coord[ptind[kk]][1]][coord[ptind[kk]][2]]==-1)
						{
							vertseq[coord[ptind[kk]][0]][coord[ptind[kk]][1]][coord[ptind[kk]][2]]=vertnumber;
							verts[vertnumber].x=coord[ptind[kk]][0];
							verts[vertnumber].y=coord[ptind[kk]][1];
							verts[vertnumber].z=coord[ptind[kk]][2];
							vertnumber++;
						}	
					}//each vertex
					jj+=3;
					faces[facenumber].a=vertseq[coord[ptind[0]][0]][coord[ptind[0]][1]][coord[ptind[0]][2]];
					faces[facenumber].b=vertseq[coord[ptind[1]][0]][coord[ptind[1]][1]][coord[ptind[1]][2]];
					faces[facenumber++].c=vertseq[coord[ptind[2]][0]][coord[ptind[2]][1]][coord[ptind[2]][2]];
				}//each triangle
				if(vertnumber+6>allocvert)
				{
					allocvert*=2;
					verts=(vertinfo *)realloc(verts,allocvert*sizeof(vertinfo));
				}
				if(facenumber+3>allocface)
				{
					allocface*=2;
					faces=(faceinfo *)realloc(faces,allocface*sizeof(faceinfo));
				}
			}//k
		}//j
	}//i
					

	verts=(vertinfo *)realloc(verts,vertnumber*sizeof(vertinfo));
	faces=(faceinfo *)realloc(faces,facenumber*sizeof(faceinfo));

	for(i=0;i<vertnumber;i++)
	{
		verts[i].atomid=vp[int(verts[i].x)][int(verts[i].y)][int(verts[i].z)].atomid;
		verts[i].iscont=false;
		if(vp[int(verts[i].x)][int(verts[i].y)][int(verts[i].z)].isbound)
			verts[i].iscont=true;
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			delete[]vertseq[i][j];
		}
	}
	
	for(i=0;i<plength;i++)
	{
		delete[]vertseq[i];
	}
	delete[]vertseq;
}//short one
//*/
//*long one
void ProteinSurface::marchingcube(int stype)
{
	int i,j,k;
	marchingcubeinit(stype);
	int ***vertseq;
	vertseq=new int**[plength];
	for(i=0;i<plength;i++)
	{
		vertseq[i]=new int*[pwidth];
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			vertseq[i][j]=new int[pheight];
		}
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			for(k=0;k<pheight;k++)
			{
				vertseq[i][j][k]=-1;
			}		
		}
	}
	if(faces!=NULL)
	{
		free(faces);
	}
	if(verts!=NULL)
	{
		free(verts);
	}
//	int allocface=20;
//	int allocvert=12;
	int allocvert=4*(pheight*plength+pwidth*plength+pheight*pwidth);
	int allocface=2*allocvert;
	facenumber=0;
	vertnumber=0;
	verts=new vertinfo[allocvert];
	faces=new faceinfo[allocface];
	
	
	int sumtype;
	int ii,jj,kk;
	int tp[6][3];
	/////////////////////////////////////////new added  normal is outer
	//face1
	for(i=0;i<1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
					tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
				    tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].b=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					||( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					}
				    else if( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face3
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face5
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<1;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone
					&& vp[i][j+1][k].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].b=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					||( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					||(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face2
	for(i=plength-1;i<plength;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
					tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					||( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					}
					else if( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face4
	for(i=0;i<plength-1;i++)
	{
		for(j=pwidth-1;j<pwidth;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone
					&& vp[i][j][k+1].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].b=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					||( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					||(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}
					else if( vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j][k+1].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}
	//face6
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=pheight-1;k<pheight;k++)
			{
				if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone
					&& vp[i][j+1][k].isdone)
				{
					tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
					tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
					tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}
				else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					||( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					||( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					||(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone))
				{
					if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}
					else if( vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}
					else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone && vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}
				
			}
		}
	}

	///////////////////////////////////////////
	for(i=0;i<plength-1;i++)
	{
		for(j=0;j<pwidth-1;j++)
		{
			for(k=0;k<pheight-1;k++)
			{
				sumtype=0;		
				for( ii=0;ii<2;ii++)
				{
					for( jj=0;jj<2;jj++)
					{
						for( kk=0;kk<2;kk++)
						{
							if(vp[i+ii][j+jj][k+kk].isdone)
								sumtype++;
						}
					}
				}//ii
				if(vertnumber+6>allocvert)
				{
					allocvert*=2;
					verts=(vertinfo *)realloc(verts,allocvert*sizeof(vertinfo));
				}
				if(facenumber+3>allocface)
				{
					allocface*=2;
					faces=(faceinfo *)realloc(faces,allocface*sizeof(faceinfo));
				}
				if(sumtype==0)
				{
					//nothing
				}//total0
				else if(sumtype==1)
				{
					//nothing
				}//total1
				else if(sumtype==2)
				{
					//nothing
				}//total2
				else if(sumtype==8)
				{
					//nothing
				}//total8
				
				else if(sumtype==3)
				{
					if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k].isdone)
					   ||(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j][k].isdone)
					   ||(vp[i][j][k+1].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone)
					   ||(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					   ||(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					   ||(vp[i][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
					   ||(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i][j][k+1].isdone)
					   ||(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone)
					   ||(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					   ||(vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k+1].isdone)
					   ||(vp[i][j][k].isdone && vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone)
					   ||(vp[i][j][k+1].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone)
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone))
					{
						if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;	
						}//11
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//12
						else if(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone&& vp[i+1][j+1][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//13
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone&& vp[i+1][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//14
						else if(vp[i][j][k+1].isdone && vp[i+1][j][k+1].isdone&& vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						}//21
						else if(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone&& vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						}//22
						else if(vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone&& vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						}//23
						else if(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone&& vp[i+1][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						}//24
						else if(vp[i][j][k].isdone && vp[i+1][j][k].isdone&& vp[i+1][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//31
						else if(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//32
						else if(vp[i][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						}//33
						else if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						}//34
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//41
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						}//42
						else if(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						}//43
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone	&& vp[i+1][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						}//44
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone ) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						}//51
						else if( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//52
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						}//53
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						}//54
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone	&& vp[i][j][k+1].isdone ) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//61
						else if(vp[i][j][k].isdone 	&& vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//62
						else if(vp[i][j][k+1].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						}//63
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone	&& vp[i][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						}//64
						for(ii=0;ii<3;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					}//no5 24
				}//total3
				else if(sumtype==4)
				{
					if((vp[i][j][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone) 
						|| (vp[i][j][k+1].isdone && vp[i+1][j][k+1].isdone
						&& vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone)
						|| (vp[i][j][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
						|| (vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)
						|| (vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone
						&& vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone)
						|| (vp[i][j][k].isdone && vp[i][j+1][k].isdone
						&& vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone))
					{
						if(vp[i][j][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
							
						}
						else if (vp[i][j][k+1].isdone && vp[i+1][j][k+1].isdone
							&& vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						}
						else if(vp[i][j][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						}
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						}
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone
							&& vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone
							&& vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}
						for(ii=0;ii<4;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
					}//no.8 6
									
				  else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone  && vp[i][j+1][k+1].isdone)//11
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone)//12
					   ||(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone && vp[i][j][k+1].isdone)//13
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k+1].isdone)//14
					   ||(vp[i][j][k+1].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j+1][k].isdone)//21
					   ||(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone)//22
					   ||(vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k].isdone)//23
					   ||(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k].isdone)//24
					   ||(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone)//31
					   ||(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)//32
					   ||(vp[i][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i+1][j+1][k].isdone)//33
					   ||(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i][j][k+1].isdone && vp[i+1][j+1][k+1].isdone)//34
					   ||(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone)//41
					   ||(vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k].isdone)//42
					   ||(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j+1][k].isdone)//43
					   ||(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone  && vp[i][j+1][k+1].isdone)//44
					   ||(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone  && vp[i+1][j][k+1].isdone)//51
					   ||( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone)//52
					   ||(vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k].isdone)//53
					   ||(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone)//54
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i][j][k+1].isdone  && vp[i+1][j+1][k+1].isdone)//61
					   ||(vp[i][j][k].isdone && vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k].isdone)//62
					   ||(vp[i][j][k+1].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j][k].isdone)//63
					   ||(vp[i][j][k].isdone && vp[i][j+1][k].isdone&& vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone))
				   {
						if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k].isdone  && vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;	
						}//11
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//12
						else if(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone&& vp[i+1][j+1][k].isdone && vp[i][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//13
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone&& vp[i+1][j][k].isdone && vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//14
						else if(vp[i][j][k+1].isdone && vp[i+1][j][k+1].isdone&& vp[i+1][j+1][k+1].isdone && vp[i][j+1][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						}//21
						else if(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						}//22
						else if(vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone&& vp[i+1][j+1][k+1].isdone && vp[i][j][k].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						}//23
						else if(vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone&& vp[i+1][j][k+1].isdone && vp[i+1][j+1][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						}//24
						else if(vp[i][j][k].isdone && vp[i+1][j][k].isdone&& vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//31
						else if(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i][j+1][k].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//32
						else if(vp[i][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone && vp[i+1][j+1][k].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						}//33
						else if(vp[i][j][k].isdone && vp[i+1][j][k].isdone && vp[i][j][k+1].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						}//34
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//41
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						}//42
						else if(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j+1][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						}//43
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone	&& vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						}//44
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone ) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						}//51
						else if( vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//52
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						}//53
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone && vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						}//54
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone	&& vp[i][j][k+1].isdone && vp[i+1][j+1][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//61
						else if(vp[i][j][k].isdone 	&& vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j+1][k].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//62
						else if(vp[i][j][k+1].isdone && vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone && vp[i+1][j][k].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						}//63
						else if(vp[i][j][k].isdone && vp[i][j+1][k].isdone	&& vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone) 
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						}//64
						for(ii=0;ii<3;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				   }//no12 24
					else if((vp[i][j][k].isdone && vp[i][j+1][k+1].isdone
						&& vp[i+1][j+1][k].isdone && vp[i][j+1][k].isdone)
						|| (vp[i][j][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone)
						|| (vp[i][j][k].isdone && vp[i][j][k+1].isdone
						&& vp[i+1][j][k].isdone && vp[i][j+1][k].isdone)
						|| (vp[i][j+1][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)
						|| (vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i+1][j+1][k+1].isdone && vp[i][j+1][k].isdone)
						|| (vp[i][j][k+1].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k+1].isdone)
						|| (vp[i][j][k].isdone && vp[i][j][k+1].isdone
						&& vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone)
						|| (vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone))
					{
						if(vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone
							&& vp[i][j][k].isdone && vp[i+1][j+1][k].isdone )
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						}//1
						else if(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone
							&& vp[i+1][j+1][k].isdone && vp[i][j][k].isdone )
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//2
						else if(vp[i][j][k].isdone && vp[i][j][k+1].isdone
							&& vp[i+1][j][k].isdone && vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//3
						else if(vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone
							&& vp[i][j+1][k].isdone && vp[i+1][j][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//4
						else if(vp[i][j+1][k].isdone && vp[i][j+1][k+1].isdone
							&& vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
						}//5
						else if(vp[i+1][j][k].isdone && vp[i+1][j][k+1].isdone
							&& vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						}//6
						else if(vp[i][j][k].isdone && vp[i][j][k+1].isdone
							&& vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						}//7
						else if(vp[i][j+1][k+1].isdone && vp[i+1][j][k+1].isdone
							&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						}//8
						for(ii=0;ii<3;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					}// no.9 8
					else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i][j][k+1].isdone)
						||(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone)
						||(vp[i][j+1][k].isdone && vp[i][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)
						||(vp[i][j+1][k].isdone && vp[i][j][k].isdone
						&& vp[i+1][j][k].isdone && vp[i][j+1][k+1].isdone)
						||(vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone
						&& vp[i+1][j][k+1].isdone && vp[i+1][j][k].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone
						&& vp[i+1][j][k+1].isdone && vp[i+1][j+1][k].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i+1][j][k+1].isdone && vp[i][j+1][k].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i][j][k+1].isdone && vp[i][j][k].isdone)
						||(vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone
						&& vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)
						||(vp[i+1][j][k+1].isdone && vp[i][j][k].isdone
						&& vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)
						||(vp[i+1][j][k+1].isdone && vp[i][j][k].isdone
						&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone)
						||(vp[i][j+1][k+1].isdone && vp[i+1][j+1][k].isdone
						&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone))
					{
						if(vp[i][j][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;	
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//1
						else if(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;	
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//2
						else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;	
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
						}//3
						else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone
							&& vp[i+1][j][k].isdone && vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;	
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//4
						else if(vp[i][j+1][k+1].isdone && vp[i][j][k+1].isdone
							&& vp[i+1][j][k+1].isdone && vp[i+1][j][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//5
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone
							&& vp[i+1][j][k+1].isdone && vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//6
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
							&& vp[i+1][j][k+1].isdone && vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//7
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
							&& vp[i][j][k+1].isdone && vp[i][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
						}//8
						else if(vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone
							&& vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//9
						else if(vp[i+1][j][k+1].isdone && vp[i][j][k].isdone
							&& vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//10
						else if(vp[i+1][j][k+1].isdone && vp[i][j][k].isdone
							&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
						}//11
						else if(vp[i][j+1][k+1].isdone && vp[i+1][j+1][k].isdone
							&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k].isdone) 
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//12
						for(ii=0;ii<4;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
					}//no.11 12
					else if((vp[i][j][k].isdone && vp[i+1][j][k].isdone
						&& vp[i][j+1][k].isdone && vp[i+1][j][k+1].isdone)
						||(vp[i][j][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)
						||(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone)
						||(vp[i][j+1][k].isdone && vp[i][j][k].isdone
						&& vp[i+1][j+1][k].isdone && vp[i][j][k+1].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone
						&& vp[i+1][j][k+1].isdone && vp[i][j][k].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i+1][j][k+1].isdone && vp[i+1][j][k].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i][j][k+1].isdone && vp[i+1][j+1][k].isdone)
						||(vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)
						||(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
						&& vp[i][j][k].isdone && vp[i][j+1][k].isdone)
						||(vp[i+1][j][k].isdone && vp[i][j][k].isdone
						&& vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone)
						||(vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone
						&& vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone)
						||(vp[i][j+1][k].isdone && vp[i+1][j+1][k].isdone
						&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k+1].isdone))
					{
						if(vp[i][j][k].isdone && vp[i+1][j][k].isdone
							&& vp[i][j+1][k].isdone && vp[i+1][j][k+1].isdone)  
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;	
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//1
						else if(vp[i][j][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i+1][j+1][k+1].isdone)  
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;	
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
						}//2
						else if(vp[i][j+1][k].isdone && vp[i+1][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i][j+1][k+1].isdone)  
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;	
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//3
						else if(vp[i][j+1][k].isdone && vp[i][j][k].isdone
							&& vp[i+1][j+1][k].isdone && vp[i][j][k+1].isdone)  
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;	
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//4
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j][k+1].isdone
							&& vp[i+1][j][k+1].isdone && vp[i][j][k].isdone)  
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
						}//5
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
							&& vp[i+1][j][k+1].isdone && vp[i+1][j][k].isdone)  
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//6
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
							&& vp[i][j][k+1].isdone && vp[i+1][j+1][k].isdone)  
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//7
						else if(vp[i+1][j][k+1].isdone && vp[i][j+1][k+1].isdone
							&& vp[i][j][k+1].isdone && vp[i][j+1][k].isdone)  
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//8
						else if(vp[i+1][j+1][k+1].isdone && vp[i][j+1][k+1].isdone
							&& vp[i][j][k].isdone && vp[i][j+1][k].isdone)  
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k;
						}//9
						else if(vp[i+1][j][k].isdone && vp[i][j][k].isdone
							&& vp[i][j][k+1].isdone && vp[i][j+1][k+1].isdone)  
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
						}//10
						else if(vp[i+1][j][k+1].isdone && vp[i][j][k+1].isdone
							&& vp[i+1][j+1][k].isdone && vp[i+1][j][k].isdone)  
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;	
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
						}//11
						else if(vp[i][j+1][k].isdone && vp[i+1][j+1][k].isdone
							&& vp[i+1][j+1][k+1].isdone && vp[i+1][j][k+1].isdone)  
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;	
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
						}//12
						for(ii=0;ii<4;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
					}//no.14 12
				}//total4
				else if(sumtype==5)
				{
					if((!vp[i+1][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						|| (!vp[i][j+1][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						|| (!vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						|| (!vp[i][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						|| (!vp[i+1][j][k+1].isdone && !vp[i][j][k].isdone && !vp[i+1][j+1][k].isdone)
						|| (!vp[i][j+1][k+1].isdone && !vp[i][j][k].isdone && !vp[i+1][j+1][k].isdone)
						|| (!vp[i+1][j+1][k+1].isdone && !vp[i+1][j][k].isdone && !vp[i][j+1][k].isdone)
						|| (!vp[i][j][k+1].isdone && !vp[i+1][j][k].isdone && !vp[i][j+1][k].isdone))
					{		
						if(!vp[i+1][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						}//1
						else if(!vp[i][j+1][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						}//2
						else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						}//3
						else if(!vp[i][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						}//4
						else if(!vp[i+1][j][k+1].isdone && !vp[i][j][k].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
						}//5
						else if(!vp[i][j+1][k+1].isdone && !vp[i][j][k].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						}//6
						else if(!vp[i+1][j+1][k+1].isdone && !vp[i+1][j][k].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						}//7
						else if(!vp[i][j][k+1].isdone && !vp[i+1][j][k].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						}//8
						for(ii=0;ii<3;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					}//no.7 8
					else if((!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone)
				   ||(!vp[i][j+1][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j][k].isdone)
				   ||(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i][j+1][k+1].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k+1].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone)
				   ||(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j][k+1].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j][k+1].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i][j][k+1].isdone)
				   ||(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone )
				   ||(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone )
				   ||(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i][j][k+1].isdone )
				   ||(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
				   ||(!vp[i][j][k+1].isdone && !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone)
				   ||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone))
				{
					if(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone)
					{
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					}//11
					else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
					}//12
					else if(!vp[i][j+1][k].isdone && !vp[i+1][j][k].isdone&& !vp[i+1][j+1][k].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[3][0]=i;tp[3][1]=j;tp[3][2]=k;
					}//13
					else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone&& !vp[i+1][j][k].isdone)
					{
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
					}//14
					else if(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone&& !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
					}//21
					else if(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone&& !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
					}//22
					else if(!vp[i][j+1][k+1].isdone && !vp[i+1][j][k+1].isdone&& !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					}//23
					else if(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone&& !vp[i+1][j][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
					}//24
					else if(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone&& !vp[i+1][j][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					}//31
					else if(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[3][0]=i;tp[3][1]=j;tp[3][2]=k;
					}//32
					else if(!vp[i][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
					}//33
					else if(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i][j][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
					}//34
					else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
					}//41
					else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
					}//42
					else if(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
					}//43
					else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
					}//44
					else if(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone ) 
					{
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
					}//51
					else if( !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
					}//52
					else if(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					}//53
					else if(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone) 
					{
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
					}//54
					else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i][j][k+1].isdone ) 
					{
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
					}//61
					else if(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
					}//62
					else if(!vp[i][j][k+1].isdone && !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[3][0]=i;tp[3][1]=j;tp[3][2]=k;
					}//63
					else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone) 
					{
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
					}//64
					for(ii=0;ii<4;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
					faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
				}//no5 24
					else if((!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k+1].isdone)//1
						||(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i][j][k+1].isdone)//2
						||(!vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i+1][j][k].isdone)//3
						||(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k].isdone)//4
						||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)//5
						||(!vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j+1][k].isdone)//6
						||(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i][j+1][k+1].isdone)//7
						||(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k].isdone)//8
						||(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)//9
						||(!vp[i+1][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k].isdone)//10
						||(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)//11
						||(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k+1].isdone))
					{
						if(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k+1;
						}//1
						else if(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k+1;
						}//2
						else if(!vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i+1][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k;
						}//3
						else if(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k;
						}//4
						else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)
						{
						    //tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							//tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							//tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							//tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k+1;
						}//5
						else if(!vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k;
						}//6
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k+1;
						}//7
						else if(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k;
						}//8
						else if(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k;
						}//9
						else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k+1;
						}//10
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k;
						}//11
						else if(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k;
						}//12
						for(ii=0;ii<5;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
						faces[facenumber].a=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].b=vertseq[tp[4][0]][tp[4][1]][tp[4][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
						
					}//no.6 12-1
					else if((!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i][j+1][k+1].isdone)//1
						||(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone)//2
						||(!vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k].isdone)//3
						||(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k].isdone)//4
						||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j][k+1].isdone)//5
						||(!vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k].isdone)//6
						||(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i][j][k+1].isdone)//7
						||(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k].isdone)//8
						||(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k].isdone)//9
						||(!vp[i+1][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k+1].isdone)//10
						||(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k].isdone)//11
						||(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k].isdone))
					{
						if(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k+1;
						}//1
						else if(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k+1;
						}//2
						else if(!vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k;
						}//3
						else if(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k;
						}//4
						else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone && !vp[i+1][j][k+1].isdone)
						{
							//tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							//tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							//tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							//tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k+1;
						}//5
						else if(!vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k;
						}//6
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k+1;
						}//7
						else if(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k;
						}//8
						else if(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k+1;
						}//9
						else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k;
						}//10
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k+1;
						}//11
						else if(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone && !vp[i+1][j][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k+1;
						}//12
						for(ii=0;ii<5;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[4][0]][tp[4][1]][tp[4][2]];
						faces[facenumber++].c=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						
					}//no.6 12-2

				}//total5
				
				else if(sumtype==6)
				{
					if((!vp[i][j][k].isdone && !vp[i+1][j][k].isdone)
						||(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone)
						||(!vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone)
						||(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone)
						||(!vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone)
						||(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						||(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone)
						||(!vp[i+1][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone)
						||(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone))
					{
						if(!vp[i][j][k].isdone && !vp[i+1][j][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						}//1
						else if(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
						}//2
						else if(!vp[i][j+1][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
						}//3
						else if(!vp[i][j][k+1].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//4
						else if(!vp[i][j][k].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
						}//5
						else if(!vp[i+1][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						}//6
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
						}//7
						else if(!vp[i][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
						}//8
						else if(!vp[i][j][k].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
						}//9
						else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						}//10
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
						}//11
						else if(!vp[i][j+1][k].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
						}//12
						for(ii=0;ii<4;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
						
					}//no.2 12	
					
					else if((!vp[i][j][k].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i+1][j][k].isdone && !vp[i][j+1][k+1].isdone)
						||(!vp[i][j+1][k].isdone && !vp[i+1][j][k+1].isdone)
						||(!vp[i+1][j+1][k].isdone && !vp[i][j][k+1].isdone))
					{
						if(!vp[i][j][k].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j+1;tp[4][2]=k;
							tp[5][0]=i+1;tp[5][1]=j;tp[5][2]=k;
						}//1
						else if(!vp[i+1][j][k].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
							tp[4][0]=i;tp[4][1]=j;tp[4][2]=k;
							tp[5][0]=i+1;tp[5][1]=j+1;tp[5][2]=k;
						}//2
						else if(!vp[i][j+1][k].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j+1;tp[4][2]=k;
							tp[5][0]=i;tp[5][1]=j;tp[5][2]=k;
						}//3
						else if(!vp[i+1][j+1][k].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
							tp[4][0]=i+1;tp[4][1]=j;tp[4][2]=k;
							tp[5][0]=i;tp[5][1]=j+1;tp[5][2]=k;
						}//4
						for(ii=0;ii<6;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
						faces[facenumber].b=vertseq[tp[4][0]][tp[4][1]][tp[4][2]];
						faces[facenumber++].c=vertseq[tp[5][0]][tp[5][1]][tp[5][2]];
					}//no.4 4
					
					else if((!vp[i][j][k].isdone && !vp[i+1][j][k+1].isdone)
						||(!vp[i+1][j][k].isdone && !vp[i][j][k+1].isdone)
						||(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone)
						||(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k+1].isdone)
						||(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i][j+1][k].isdone && !vp[i][j][k+1].isdone)
						||(!vp[i][j][k].isdone && !vp[i][j+1][k+1].isdone)
						||(!vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						||(!vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						||(!vp[i][j][k].isdone && !vp[i+1][j+1][k].isdone)
						||(!vp[i+1][j][k].isdone && !vp[i][j+1][k].isdone))
					{
						if(!vp[i][j][k].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k+1;
						}//1
						else if(!vp[i+1][j][k].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
						}//2
						else if(!vp[i+1][j][k].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k+1;
						}//3
						else if(!vp[i+1][j+1][k].isdone && !vp[i+1][j][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
						}//4
						else if(!vp[i+1][j+1][k].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//5
						else if(!vp[i][j+1][k].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k;
						}//6
						else if(!vp[i][j+1][k].isdone && !vp[i][j][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//7
						else if(!vp[i][j][k].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k;
						}//8
						else if(!vp[i][j][k+1].isdone && !vp[i+1][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
							tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
							tp[3][0]=i+1;tp[3][1]=j+1;tp[3][2]=k;
						}//9
						else if(!vp[i+1][j][k+1].isdone && !vp[i][j+1][k+1].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
							tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
							tp[3][0]=i;tp[3][1]=j+1;tp[3][2]=k;
						}//10
						else if(!vp[i][j][k].isdone && !vp[i+1][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
							tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
							tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[3][0]=i;tp[3][1]=j;tp[3][2]=k+1;
						}//11
						else if(!vp[i+1][j][k].isdone && !vp[i][j+1][k].isdone)
						{
							tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
							tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
							tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;
							tp[3][0]=i+1;tp[3][1]=j;tp[3][2]=k+1;
						}//12
						for(ii=0;ii<4;ii++)
						{
							if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
							{
								vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
								verts[vertnumber].x=tp[ii][0];
								verts[vertnumber].y=tp[ii][1];
								verts[vertnumber].z=tp[ii][2];
								vertnumber++;
							}
						}
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
						faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
						faces[facenumber].b=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
						faces[facenumber++].c=vertseq[tp[3][0]][tp[3][1]][tp[3][2]];
					}//no.3 12
					
				}//total6
				
				else if(sumtype==7)
				{
					if(!vp[i][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k+1;
					}//1
					else if(!vp[i+1][j][k].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k;		
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k+1;
					}//2
					else if(!vp[i+1][j+1][k].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k;		
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k+1;
					}//3
					else if(!vp[i][j+1][k].isdone)
					{				
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k;
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k+1;
					}//4
					else if(!vp[i][j][k+1].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j+1;tp[1][2]=k+1;		
						tp[2][0]=i;tp[2][1]=j;tp[2][2]=k;
					}//5
					else if(!vp[i+1][j][k+1].isdone)
					{
						tp[0][0]=i+1;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[1][0]=i;tp[1][1]=j;tp[1][2]=k+1;		
						tp[2][0]=i+1;tp[2][1]=j;tp[2][2]=k;
					}//6
					else if(!vp[i+1][j+1][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j+1;tp[0][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j;tp[1][2]=k+1;			
						tp[2][0]=i+1;tp[2][1]=j+1;tp[2][2]=k;
					}//7
					else if(!vp[i][j+1][k+1].isdone)
					{
						tp[0][0]=i;tp[0][1]=j;tp[0][2]=k+1;
						tp[1][0]=i+1;tp[1][1]=j+1;tp[1][2]=k+1;		
						tp[2][0]=i;tp[2][1]=j+1;tp[2][2]=k;
					}//8
					for(ii=0;ii<3;ii++)
					{
						if(vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]==-1)
						{
							vertseq[tp[ii][0]][tp[ii][1]][tp[ii][2]]=vertnumber;
							verts[vertnumber].x=tp[ii][0];
							verts[vertnumber].y=tp[ii][1];
							verts[vertnumber].z=tp[ii][2];
							vertnumber++;
						}
					}
					faces[facenumber].a=vertseq[tp[0][0]][tp[0][1]][tp[0][2]];
					faces[facenumber].b=vertseq[tp[1][0]][tp[1][1]][tp[1][2]];
					faces[facenumber++].c=vertseq[tp[2][0]][tp[2][1]][tp[2][2]];
				}//total7
						
			}//every ijk
		}//j
	}//i
	verts=(vertinfo *)realloc(verts,vertnumber*sizeof(vertinfo));
	faces=(faceinfo *)realloc(faces,facenumber*sizeof(faceinfo));
	for(i=0;i<vertnumber;i++)
	{
		verts[i].atomid=vp[int(verts[i].x)][int(verts[i].y)][int(verts[i].z)].atomid;
		verts[i].iscont=false;
		if(vp[int(verts[i].x)][int(verts[i].y)][int(verts[i].z)].isbound)
			verts[i].iscont=true;
	}
	for(i=0;i<plength;i++)
	{
		for(j=0;j<pwidth;j++)
		{
			delete[]vertseq[i][j];
		}
	}
	
	for(i=0;i<plength;i++)
	{
		delete[]vertseq[i];
	}
	delete[]vertseq;
}
//*/

