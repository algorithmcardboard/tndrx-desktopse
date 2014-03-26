#include"tndrx_db.h"

struct tndrx_db* read_db()
{
	struct tndrx_db *t;
	int *watch;
	char *str;
	str = (char*)malloc(520);
	watch = (int*)malloc(sizeof(int))
	t = (struct tndrx_db*)malloc(sizeof(struct tndrx_db));
	if(filelist != NULL)
	{
		fread(watch,sizeof(int),1,filelist);
		fgets(str,520,filelist);
		t->watch_desc = watch[0];
		t->val_ftype = str[0];
		strcpy(t->name,(str+1))
		return t;
	}
	else
	{
		t->watch_desc = -1;
		t->val_ftype = 0;
		strcpy(t->name,"");
		return t;
	}
}

void write_db(struct tndrx_db *t)
{
	fwrite(t->);
}
