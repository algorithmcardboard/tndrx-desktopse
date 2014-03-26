#include"initdb.h"
#include"monitor.h"
#include<errno.h>
#include"tndrx_db.h"

static int t;

void user_logged(char *username)
{
	FILE *f;
	struct utmp *u;
	u = (struct utmp*)malloc(sizeof(struct utmp));
	f = fopen("/var/run/utmp","r");
	while(fread(u,sizeof(struct utmp),1,f))
		if(!strcmp(u->ut_line,"tty7"))
			strcpy(username,u->ut_user);
	fclose(f);
}

void initiate()
{
	char filename[100],username[50],file_temp[100];
	strcpy(username,"anirudh");
	user_logged(username);
	if(strcmp(username,"root"))
	{
		strcpy(filename,"/home/");
		strcat(filename,username);
		strcat(filename,"/Documents/linear");
		if(opendir(filename) == NULL)
			mkdir(filename,0700);
		strcat(filename,"/db");
		if(opendir(filename) == NULL)
			mkdir(filename,0700);
		strcpy(file_temp,filename);
		strcat(file_temp,"/error");
		strcat(filename,"/filelist");
	}
	else
	{
		strcpy(filename,"/root/Documents/linear/db");
		strcpy(file_temp,filename);
		strcat(file_temp,"/error");
		strcat(filename,"/filelist");
		if(opendir("/root/Documents/linear") == NULL)
			mkdir("/root/Documents/linear",0700);
		if(opendir("/root/Documents/linear/db") == NULL)
			mkdir("/root/Documents/linear/db",0700);
	}
	filelist = fopen(filename,"w");
	fil = fopen(file_temp,"w");
}

void buildDB()
{
	count = 0;
	int path_pos,i,watch;
	char temp[15];
	unsigned char message = 0X14;
	inotify_add_watch(fd,"/",IN_CREATE | IN_DELETE | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO);
	for(i = 0;i < 17;i++)
	{
		strcpy(temp,"/");
		strcat(temp,list[i]);
		if(!access(temp,R_OK))
		{
			if((watch = inotify_add_watch(fd,temp,IN_CREATE | IN_DELETE | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO )) == -1)
			{
				fprintf(fil,"%d\t%s\t%s\n",t,temp,strerror(errno));
				watch = inotify_add_watch(fd,temp,IN_CREATE | IN_DELETE_SELF | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO );
			}
			t = watch;
			fwrite(&watch,sizeof(watch),1,filelist);
			fprintf(filelist,"%c%s\n",message,list[i]);
			count++;
		}
		recursive_call(temp);
	}
	fclose(fil);
	fclose(filelist);
	printf("Number of files %d",count);
}

void recursive_call(char *dirname)
{
	DIR *dirpointer;
	struct dirent *dirreadpointer;
	int path_pos,watch;
	char *filename,*temp,*dname;
	dname = (char*)malloc(1 + strlen(dirname));
	filename = (char*)malloc(512);
	temp = (char*)malloc(512 + strlen(dirname));
	strcpy(dname,dirname);
	unsigned char message;
	unsigned char i = 0;
	if(!access(dname,R_OK))
	{
		dirpointer = opendir(dname);
		while((dirreadpointer = readdir(dirpointer)) != NULL)
		{
			if(strcmp(dirreadpointer->d_name,".") && strcmp(dirreadpointer->d_name,".."))
			{
				strcpy(filename,dirreadpointer->d_name);
				strcpy(temp,dname);
				strcat(temp,"/");
				strcat(temp,filename);
				message = 0X10|dirreadpointer->d_type;
				if(!access(temp,R_OK))
				{
					watch = -1;
					if((watch = inotify_add_watch(fd,temp,IN_CREATE | IN_DELETE_SELF | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO )) == -1)
					{
						fprintf(fil,"%d\t%s\t%s\n",t,temp,strerror(errno));
						watch = inotify_add_watch(fd,temp,IN_CREATE | IN_DELETE_SELF | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO );
					}
					t = watch;
					fwrite(&watch,sizeof(watch),1,filelist);
					fprintf(filelist,"%c%s\n",message,filename);
					count++;
				}
				if(dirreadpointer->d_type == 4)
				{
					recursive_call(temp);
				}
			}
		}
		closedir(dirpointer);
	}
	message = 0X1F;
	watch = -1;
	fwrite(&watch,sizeof(watch),1,filelist);	
	fprintf(filelist,"%c\n",message);
	free(dname);
	free(temp);
	free(filename);
}

int main()
{
	int *fd;
	initiate();
	buildDB(fd);
	return 0;
}
