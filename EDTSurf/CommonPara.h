/*//////////////////////////////////////////////////////////////// 
Permission to use, copy, modify, and distribute this program for 
any purpose, with or without fee, is hereby granted, provided that
the notices on the head, the reference information, and this
copyright notice appear in all copies or substantial portions of 
the Software. It is provided "as is" without express or implied 
warranty.
*////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define maxnumchains 120

static char aad1[]= {'A','C','D','E','F',
'G','H','I','K','L',
'M','N','P','Q','R',
'S','T','V','W','Y',
'J','B','Z','X','X','U'};

static char aad3[][4] = {"ALA","CYS","ASP","GLU","PHE",
"GLY","HIS","ILE","LYS","LEU",
"MET","ASN","PRO","GLN","ARG",
"SER","THR","VAL","TRP","TYR",
"XLE","ASX","GLX","UNK","XAA","SEC"};

//static double rasrad[ ]={1.872,1.872,1.507,1.4,1.848,1.1,1.88,1.872,1.507,1.948,1.5, 1.4, 1.1};//lsms
static double rasrad[ ]={1.90,1.88,1.63,1.48,1.78,1.2,1.87,1.96,1.63,0.74,1.8, 1.48, 1.2};//liang
//                         ca   c    n    o    s    h   p   cb    ne   fe  other ox  hx

static unsigned char color[][4]={
	226, 14,226, 88,  255,255,255, 88,    0,  0,128, 88,  255,  0,  0, 88,  
    128,128,  0, 88,  128,128,128, 88,  255,255,  0, 88,  255,255,255, 88,  
      0,  0,128, 88,  128,  0,128, 88,    0,255,255, 88,  255,  0,  0, 88,  
	128,128,128, 88,   88,235,108,188,    0,128,128,88};
static unsigned char mycolor[19][4]={128,128,128,220, 
255,0,0,220,     255,128,0,220,   255,255,0,220,
0,255,0,220,     0,0,255,220,     0,255,255,220,  
255,0,255,220,   0,255,128,220,   128,0,255,220,

255,128,128,220,   0,128,255,220,     0,128,128,220,   
128,255,128,220,   128,128,255,220,   255,0,128,220,   
128,128, 0,220,    128,255,0,220,     128,0,128,220,
	};
static char nb[26][3]={1,0,0, -1,0,0, 0,1,0, 0,-1,0, 0,0,1, 0,0,-1, 
1,1,0, 1,-1,0, -1,1,0, -1,-1,0, 1,0,1, 1,0,-1, -1,0,1, -1,0,-1, 0,1,1, 0,1,-1, 0,-1,1, 0,-1,-1,
1,1,1, 1,1,-1, 1,-1,1, -1,1,1, 1,-1,-1, -1,-1,1, -1,1,-1, -1,-1,-1};
static double asarea2[]= {
240.270, 267.707, 283.510, 315.039, 363.235, 
215.365, 333.600, 312.879, 347.779, 320.895, 
323.209, 288.210, 269.461, 317.071, 379.887,
249.458, 275.129, 286.960, 404.584, 376.261, 
300,300,300,300,300,300};
typedef struct point3d
{
	double x,y,z;
}point3d;
typedef struct	 atom	 // Structure For An Object
{  
	char	   simpletype;		// atom hetatm								
	int        seqno;
	char       detailtype[5];  // cg
	char       detail;         // 1 to 12
	char       residue[3];     // leu
	char       residueid; // 1-21
	char       chainid;        // a b or null
	int        resno; // 408
	float	   x,y,z;
	float      occu,tempe;
	char       alt,ins;
	unsigned char inout;//1 out 2 middle 3 inner
} atom;	
typedef struct	 paratrans										
{
	bool    isorigx,isscale,ismtrix;
	double	origxn[3][4];
	double  scalen[3][4];
	double  mtrixn[3][4];
} paratrans;
typedef struct	 helixlink										
{
	char initid,endid;
	char initchainid,endchainid;
	int  initindex,endindex;
	char helixtype;
	int helixlength;
} helixlink;
typedef struct	 sheetlink										
{
	char initid,endid,currid,previd;
	char initchainid,endchainid,currchainid,prevchainid;
	int  initindex,endindex,currindex,previndex;
	char curratom[4],prevatom[4];
	char sheettype;
} sheetlink;
typedef struct	 turnlink										
{
	char initid,endid;
	char initchainid,endchainid;
	int  initindex,endindex;
} turnlink;
typedef struct	 linklink										
{
	char initid[3],endid[3];
	char initchainid,endchainid;
	int  initindex,endindex;
	char initatom[4],endatom[4];
} linklink;
typedef struct	 ssbondlink										
{
	char initchainid,endchainid;
	int  initindex,endindex;
} ssbondlink;
typedef struct	 hydbndlink										
{
	char initid,endid;
	char initchainid,endchainid;
	int  initindex,endindex;
	char initatom[4],endatom[4];
} hydbndlink;

typedef struct	 sltbrglink										
{
	char initid,endid;
	char initchainid,endchainid;
	int  initindex,endindex;
	char initatom[4],endatom[4];
}sltbrglink; 

typedef struct	 cispeplink										
{
	char initid,endid;//amino2add3index
	char initchainid,endchainid;//chainid
	int  initindex,endindex;//seqnum
}cispeplink;
typedef struct sitelink
{
	char sitename[3];
	char sitenum;
	char aminoid[4];
	char chainid[4];
	int  amiindex[4];	
}sitelink;
typedef struct segment
{
	int init;
	int term;
}segment;

typedef struct prochain
{
	segment chainseg;
	char chainid;
}prochain;
typedef struct promodel
{
	prochain procha[maxnumchains];
	char  nchain;	
}promodel;
typedef struct boneinfo
{
	int indn,indca,indc,indo;
	int resind;
	char resid;
	int istart,iend;
}boneinfo;
////////////////////////////////

typedef struct volumepixel
{
	int atomid;
	float distance;
	bool inout;
	bool isbound;
	bool isdone;	
}volumepixel;
typedef struct voxel										
{
	int ix,iy,iz;	
}voxel;
typedef struct point3s										
{
	float x,y,z;	
}point3s;
typedef struct voxel2										
{
	short int ix,iy,iz;	
}voxel2;
typedef struct faceinfo
{
	int a,b,c;
	point3d pn;
	double area;
	bool inout;//interior true
}faceinfo;
typedef struct vertinfo
{
	double x,y,z;
	point3d pn;
	double area;
	int atomid;
	bool inout,iscont;//is concave surface
}vertinfo;
int jacobi(int n,double a[],double d[],double v[]);
int aminoid(char aminoname);


















































