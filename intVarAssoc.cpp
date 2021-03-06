#include <stdlib.h>
#ifndef MSDOS 
#include <unistd.h>
#endif
#include "geneVarUtils.hpp"

int main(int argc,char *argv[])
{
	char fn[100],fn2[100],line[1000],*testName,intStr[100];
	int i,first;
	FILE *fp,*fi;
	gvaParams gp;
	analysisSpecs spec;

	dcerror.warn();

	fp=fopen(argv[1],"r");
	gp.input(fp,spec);
	fclose(fp);
	masterLocusFile vf(gp.nCc[0]+gp.nCc[1]);

	fi=fopen(argv[2],"r");
	testName=argv[3];
	sprintf(fn,"gva.%s.db",testName);
	sprintf(fn2,"gva.%s.vdx",testName);
	unlink(fn);
	unlink(fn2);
	vf.openFiles(fn,fn2);
	first=1;
	while (fgets(line,999,fi) && sscanf(line,"%s",intStr)==1)
	{
		printf("%s\n",intStr);
		int ff=0;
		for (i=0;i<gp.nCc[0];++i)
		{
			sprintf(fn,"iva.%s.cont.%d.vcf",testName,i+1);
			sprintf(line,"tabix %s %s %s%s %s %s",gp.ccFn[0][i],first==1?"-h":"",spec.addChrInVCF[ff++]?"chr":"",intStr,first?">":">>",fn);
			system(line);
		}
		for (i=0;i<gp.nCc[1];++i)
		{
			sprintf(fn,"iva.%s.case.%d.vcf",testName,i+1);
			sprintf(line,"tabix %s %s %s%s %s %s",gp.ccFn[1][i],first==1?"-h":"",spec.addChrInVCF[ff++]?"chr":"",intStr,first?">":">>",fn);
			system(line);
		}
		first=0;
	}
	for (i=0;i<gp.nCc[0];++i)
		{
			sprintf(fn,"iva.%s.cont.%d.vcf",testName,i+1);
			vf.addLocusFile(fn,VCFFILE);
			vf.readLocusFileEntries(fn,spec,0);
		}
	for (i=0;i<gp.nCc[1];++i)
		{
			sprintf(fn,"iva.%s.case.%d.vcf",testName,i+1);
			vf.addLocusFile(fn,VCFFILE);
			vf.readLocusFileEntries(fn,spec,1);
		}

	sprintf(fn,"gva.%s",testName);

	vf.writeOldScoreAssocFiles(fn,gp.wf,gp.wFunc,gp.useFreqs,gp.nSubs,1,gp.writeComments,spec);

	sprintf(line,"scoreassoc %s.par %s.dat %s.sao",fn,fn,fn);
	if (gp.writeScoreFile==1)
		sprintf(strchr(line,'\0')," %s.sco",fn);
	system(line);

	return 0;
}