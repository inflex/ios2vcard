#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int splitfiles = 0, foc = 1;
int obfusicate = 0;
int reverse_names = 0;
char *inputfile = NULL;

/*-----------------------------------------------------------------\
  Date Code:	: 20160723-203321
  Function Name	: int
  Returns Type	: static
  	----Parameter List
	1. callback(void *NotUsed, 
	2.  int argc, 
	3.  char **argv, 
	4.  char **azColName, 
	------------------
Exit Codes	: 
Side Effects	: 
--------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	char *n="", *first, *last, *num, *title, *t;
	char firsto[1024], lasto[1024], numo[1024];
	char foname[1024];
	FILE *fo;

	title = argv[0]?argv[0]:n;
	first = argv[1]?argv[1]:n;
	last = argv[2]?argv[2]:n;
	num = argv[3]?argv[3]:n;

	if (obfusicate) {
		char *p, *q;
		p = first; q = firsto; while (*p++) { *q = *p +1; if (*p == ' ') *q = *p; q++; } *q = '\0';
		p = last; q = lasto; while (*p++) { *q = *p +1;  if (*p == ' ') *q = *p; q++; } *q = '\0';
		p = num; q = numo; while (*p++) { *q = *p +1; if (*p == ' ') *q = *p; if (*q > '9') *q = '3'; q++; } *q = '\0';
	}


	if (splitfiles) {
		snprintf(foname,sizeof(foname),"%04d-%s_%s.vcf", foc, first, last);
		fo = fopen(foname, "w");
		if (!fo) {
			fprintf(stderr, "Cannot open '%s' for writing\n", foname);
			return 0;
		}
	} else {
		fo = stdout;
	}

	if (reverse_names) {
		t = first;
		first = last;
		last = t;
	}

	foc++;
	fprintf(fo,"BEGIN:VCARD\r\nVERSION:3.0\r\nN:%s;%s;;%s\r\nFN:%s %s\r\nTEL;TYPE=WORK,VOICE:%s\r\nEND:VCARD\r\n"
			,obfusicate?firsto:first
			,obfusicate?lasto:last
			,title

			,obfusicate?firsto:first
			,obfusicate?lasto:last

			,obfusicate?numo:num
		   );
	if ((splitfiles)&&(fo)) fclose(fo);
	return 0;
}

/*-----------------------------------------------------------------\
  Date Code:	: 20160723-203317
  Function Name	: parse_parameters
  Returns Type	: int
  	----Parameter List
	1. int argc, 
	2.  char **argv , 
	------------------
Exit Codes	: 
Side Effects	: 
--------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int parse_parameters( int argc, char **argv ) {

	int i;

	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-') {
			/* parameter */
			switch (argv[i][1]) {
				case 'i':
					/* set input file */
					i++;
					if (i < argc) inputfile=argv[i];
					else {
						fprintf(stderr,"Insufficient parameters\n");
						exit(1);
					}
					break;

				case  'x':
					obfusicate = 1;
					break;

				case 'r':
					reverse_names = 1;
					break;

				case 's':
					splitfiles=1;
					/* separate output files */
					break;

				default:
					break;
			} // switch
		}
	}

	return 0;
}


/*-----------------------------------------------------------------\
  Date Code:	: 20160723-203314
  Function Name	: main
  Returns Type	: int
  	----Parameter List
	1. int argc, 
	2.  char **argv, 
	------------------
Exit Codes	: 
Side Effects	: 
--------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int main(int argc, char **argv){
	sqlite3 *db;
	char sql[1024];
	char *zErrMsg = 0;
	int rc;

	if( argc<=2 ){
		fprintf(stderr, "Usage: %s -i <DATABASE> [ -s ] [ -x ][ -r ]\n"
				"\t-s : Write each vCard to separate file\n"
				"\t-x : obfusicate the data\n"
				"\t-r : Reverse first/last name\n"
				, argv[0]
				);
		return(1);
	}

	parse_parameters(argc, argv);

	if (inputfile) {
		rc = sqlite3_open(inputfile, &db);
		if( rc ){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(1);
		}
		snprintf(sql,sizeof(sql),"select ABPerson.prefix, ABPerson.first,ABPerson.last, ABMultiValue.value from ABPerson,ABMultiValue where ABMultiValue.record_id=ABPerson.ROWID");
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
	}
	return 0;
}

