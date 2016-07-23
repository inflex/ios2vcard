#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	char *n="", *first, *last, *num, *title;
	for(i=0; i<argc; i++){

		title = argv[i]?argv[i]:n;
		first = argv[i+1]?argv[i+1]:n;
		last = argv[i+2]?argv[i+2]:n;
		num = argv[i+3]?argv[i+3]:n;

		printf("BEGIN:VCARD\r\n\
VERSION:3.0\r\n\
N:%s;%s;;%s\r\n\
FN:%s %s\r\n\
TEL;TYPE=WORK,VOICE:%s\r\n\
END:VCARD\r\n"
		,first
		,last
		,title

		,first
		,last

		,num
		);
		i+=3;
	}
	return 0;
}

int main(int argc, char **argv){
	sqlite3 *db;
	char sql[1024];
	char *zErrMsg = 0;
	int rc;

	if( argc!=2 ){
		fprintf(stderr, "Usage: %s DATABASE \n", argv[0]);
		return(1);
	}
	rc = sqlite3_open(argv[1], &db);
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
	return 0;
}

