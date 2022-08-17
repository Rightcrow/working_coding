#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ini.h"

#include "dictionary.h"
#include "iniparser.h"

#define FILENAME	"./CamPIR.ini"

void creat_CamPIR_ini(void)
{
	if (access(FILENAME, F_OK | W_OK | R_OK) >= 0)
	{
		return;
	}

	FILE *ini;
	
	if ((ini = fopen(FILENAME, "w"))==NULL){
		fprintf(stderr,"file is ok: %s\n",FILENAME);
		return;
	}

	fprintf(ini,
	"#\n"
	"# This is an CAMERA and PIR of ini file\n"
	"#\n"
	"[camera]\n"
	"\n"
	"mode		= 2 ;\n"
	"picturesize = 1 ;\n"
	"videosize 	= 1 ;\n"
	"pictimes	= 1 ;\n"
	"videotimes	= 5 ;\n"
	"gaptimes	= 10;\n"
	"sensitivity= 3 ;\n"
	"timestamp 	= 1 ;\n"
	"retimes	= 60 ;\n"
	"\n");
	
	fclose(ini);

	return;
}

int save_CamPIR_int(char *key_, char *value_)
{
	char cam[] = "camera:";
	char *name = (char*)malloc(strlen(cam)+strlen(key_));
	sprintf(name, "%s%s", cam, key_);
	if (access(FILENAME, F_OK | W_OK | R_OK) < 0)
	{
		creat_CamPIR_ini();
	}

	dictionary *ini;

	ini = iniparser_load(FILENAME);
	if (ini == NULL)
	{
		fprintf(stderr, "cannot parse file: %s\n", FILENAME);
		return -1;
	}
	printf("\n ***************ini is open********\n");

	iniparser_set(ini, name, value_);

	FILE *fp = fopen(FILENAME, "w");
	if (NULL != fp)
	{
		iniparser_dump_ini(ini, fp);
		fclose(fp);
	}
	iniparser_freedict(ini);

	free(name);
	name = NULL;

	return 0;

}

int get_CamPIR_int(char* key)
{
	char cam[] = "camera:";
	char *name = (char*)malloc(strlen(cam)+strlen(key));
	sprintf(name, "%s%s", cam, key);
	if (access(FILENAME, F_OK | W_OK | R_OK) < 0)
	{
		creat_CamPIR_ini();
	}


	dictionary *ini;

	ini = iniparser_load(FILENAME);
	if (NULL == ini)
	{
		fprintf(stderr, "cannot parse file: %s\n", FILENAME);
		return 0;
	}

	const char *value = iniparser_getstring(ini, name, NULL);
	int param = atoi(value);
	printf("\n value: %s\n", value);

	iniparser_freedict(ini);
	free(name);
	name = NULL;

	return param;
}


/*
int main(void)
{
	char key[] = "videotimes";
		
	int param = get_CamPIR_int(key);
	

	printf("video time :%d\n", param);
	
	return 0;
}
*/