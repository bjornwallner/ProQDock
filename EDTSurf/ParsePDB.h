/*//////////////////////////////////////////////////////////////// 
Permission to use, copy, modify, and distribute this program for 
any purpose, with or without fee, is hereby granted, provided that
the notices on the head, the reference information, and this
copyright notice appear in all copies or substantial portions of 
the Software. It is provided "as is" without express or implied 
warranty.
*////////////////////////////////////////////////////////////////
// ParsePDB.h: interface for the ParsePDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEPDB_H__0A306B68_CEEB_4EEF_8D55_4CCE700E11A3__INCLUDED_)
#define AFX_PARSEPDB_H__0A306B68_CEEB_4EEF_8D55_4CCE700E11A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ParsePDB  
{
public:
	ParsePDB();
	char pdbname[5];
	int headnumchain;//number of chains
	char headidchain[maxnumchains];// id of chains
	int indexchain,numinchain[maxnumchains];// num of aminos in each chain
	char *aminoinchain[maxnumchains];// aminos in each chain
	paratrans pt;
	int numhelix,numsheet,numturn,numlink;
	int allochelix,allocsheet,allocturn,alloclink;
	helixlink *helixl;
	sheetlink *sheetl;
	turnlink *turnl;
	linklink *linkl;
	int numssbond,numsltbrg,numcispep,numhydbnd,numsite;
	int allocssbond,allocsltbrg,alloccispep,allochydbnd,allocsite;
	ssbondlink *ssbondl;
	sltbrglink *sltbrgl;
	cispeplink *cispepl;
	hydbndlink *hydbndl;
	sitelink *sitel;
	int numpromod;
	int allocpromod;
	promodel *promod;
	bool flagfirstmodel;
	bool flagbeginmodel;
	int numproseq;
	int allocproseq;
	atom *proseq;
	int aminoid(char *aminoname);
	int aminoid(char aminoname);
	int dealline(char *string);
	bool loadpdb(char *pdbfile);
	void splitpdb();
	int numbb,effbb;
	boneinfo *bb;
	void extractbb(int model, int chain,int type);
	void PCA();
	virtual ~ParsePDB();
};

#endif // !defined(AFX_PARSEPDB_H__0A306B68_CEEB_4EEF_8D55_4CCE700E11A3__INCLUDED_)
