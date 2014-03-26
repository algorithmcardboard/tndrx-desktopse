#include"demon.h"

void wait_login()
{
	unsigned char flag = 0;
	struct utmp *u;
	u = (struct utmp*)malloc(sizeof(struct utmp));
	while(!flag)
	{
		FILE *f;
		f = fopen("/var/run/utmp","r");
		if(f == NULL)
			printf("error");
		while(fread(u,sizeof(struct utmp),1,f) ? 1 : 0)
		{
			if((u->ut_type == 7) && (!strcmp(u->ut_line,"tty7")))
				flag = 1;
		}
		fclose(f);
		if(!flag)
			sleep(10);
		else
			break;
	}
}
