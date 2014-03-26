#include<stdio.h>

#ifndef _TNDRX_DB_H
#define _TNDRX_DB_H

FILE *filelist;

struct tndrx_db
{
	int watch_desc;
	unsigned char val_ftype;
	char name[520];
};

struct tnrrx_db* read_db();
void write_db(struct tndrx_db*);

#endif
