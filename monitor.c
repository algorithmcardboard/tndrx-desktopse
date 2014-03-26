#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<utmp.h>
#include"final/monitor.h"
#include"final/initdb.c"
#include"final/demon.c"

char username[50];

long int search1(int,char*);
long int search2(int,char*);

int main()
{
	int i,len,watch;
	pid_t pid, sid;
	FILE *log,*recent,*user_watch,*f;
	char buffer[BUF_LEN],file_name[100],file_name1[100];
	pid = fork();
	if(pid < 0)
		exit(EXIT_FAILURE);
	if(pid > 0)
		exit(EXIT_SUCCESS);
	umask(0);
	sid = setsid();
	if(sid < 0)
		exit(EXIT_FAILURE);
	if(chdir("/") < 0)
		exit(EXIT_FAILURE);
	close(stdin);
	close(stdout);
	close(stderr);

	wait_login();

//-----------------------------------------

	log = fopen("/var/log/TndRX.log","w");
	fprintf(log,"refreshing");
	fclose(log);
	user_watch = fopen("/proc/sys/fs/inotify/max_user_watches","w");
	fprintf(user_watch,"500000");
	fclose(user_watch);
	fd = inotify_init();
	initiate();
	buildDB();

//-----------------------------------------

	log = fopen("/var/log/TndRX.log","w");
	fprintf(log,"monitoring");
	fclose(log);
	user_logged(username);
	if(strcmp(username,"root"))
	{
		strcpy(file_name,"/home/");
		strcat(file_name,username);
	}
	else
		strcpy(file_name,"/root");
	strcat(file_name,"/Documents/linear/db/");
	strcpy(file_name1,file_name);
	strcat(file_name1,"filelist");
	strcat(file_name,"recent");
	recent = fopen(file_name,"w");
	fclose(recent);
	struct inotify_event *event;
	int cnt;
	char temp[520],t[1000],path[1050],del_str[1050];
	long int pos;
	unsigned char message;
	while(1)
	{
		if((len = read(fd, buffer, BUF_LEN)) > 0)
		{
			i = 0;
			while(i < len)
			{
				event = (struct inotify_event*) &buffer[i];
				if(event->len)
				{
					if(event->mask & IN_CREATE)
					{
						watch = -1;
						if(search1(event->wd,path) == -1)
							search2(event->wd,path);
						recent = fopen(file_name,"a+");
						strcpy(temp,path);
						watch = inotify_add_watch(fd,strcat(temp,event->name),IN_CREATE | IN_DELETE_SELF | IN_ATTRIB | IN_MOVED_FROM | IN_MOVED_TO);
						fwrite(&watch,sizeof(int),1,recent);
						if(event->mask & IN_ISDIR)
							message = 0X14;
						else	message = 0X11;
						fprintf(recent,"%c%s%s\n",message,event->name,path);
						fclose(recent);
					}
					else if(event->mask & IN_DELETE_SELF)
					{
						if((pos = search1(event->wd,path)) != -1)
						{
							f = fopen(file_name,"r+");
							fseek(f,pos,SEEK_SET);
							fread(&watch,sizeof(int),1,f);
							message = 0X00;
							fprintf(f,"%c",message);
							fclose(f);
						}
						else if((pos = search2(event->wd,path)) != -1)
						{
							f = fopen(file_name1,"r+");
							fseek(f,pos,SEEK_SET);
							fread(&watch,sizeof(int),1,f);
							message = 0X00;
							fprintf(f,"%c",message);
							fclose(f);
						}
					}
					else if(event->mask & IN_ATTRIB)
					{
					}
					else if(event->mask & IN_MOVED_FROM)
					{
					}
					else if(event->mask & IN_MOVED_TO)
					{
					}
				}
				i += (EVENT_SIZE + event->len);
			}
		}
	}
}

long int search1(int wd,char *path)
{
	FILE *f,*recent,*j;
	int watch;
	long int pos;
	char *t,*target;
	t = (char*)malloc(100);
	target = (char*)malloc(1050);
	strcpy(path,"/");
	user_logged(username);
	if(strcmp(username,"root"))
	{
		strcpy(t,"/home/");
		strcat(t,username);
	}
	else
		strcpy(t,"/root");
	strcat(t,"/Documents/linear/db/recent");
	f = fopen(t,"r");
//	j = fopen("/root/Documents/linear/db/jp","w");
//	fprintf(j,"1\n");
//	fclose(j);
	while(!feof(f))
	{
		pos = ftell(f);
		fread(&watch,sizeof(int),1,f);
//			printf("error");
		fgets(target,1050,f);
//	j = fopen("/root/Documents/linear/db/jp","a+");
//		fprintf(j,"%d\t%s\n",watch,target+1);
//	fclose(j);
		if(target[0] == '\0')	break;
		if((target[0] & 0XF0))
		{
			if(watch == wd)
			{
				strcpy(path,strstr(target,"/"));
				break;
			}
		}
	}
	if(feof(f))
		pos = -1;
	fclose(f);
	return pos;
}

long int search2(int wd,char *path)
{
	FILE *f,*recent,*j;
	int i,watch;
	long int pos;
	char *t,*target;
	t = (char*)malloc(1050);
	target = (char*)malloc(1050);
	strcpy(path,"/");
	user_logged(username);
	if(strcmp(username,"root"))
	{
		strcpy(t,"/home/");
		strcat(t,username);
	}
	else
		strcpy(t,"/root");
	strcat(t,"/Documents/linear/db/filelist");
	f = fopen(t,"r");
//	j = fopen("/root/Documents/linear/db/jp","w");
//	fprintf(j,"2\n");
//	fclose(j);
//	j = fopen("/root/Documents/linear/db/jp","a+");
	while(!feof(f))
	{
		pos = ftell(f);
		fread(&watch,sizeof(int),1,f);
		fgets(target,520,f);
//		fprintf(j,"%d\t%s\n",watch,target+1);
		if(target[0] == '\0')	break;
		target[strlen(target) - 1] = '\0';
		if((target[0] & 0XF0))
		{
			if((target[0] & 0X0F) == 4)
			{
				strcat(path,(target+1));
				strcat(path,"/");
			}
			if((target[0] & 0X0F) == 15)
			{
				i = strlen(path) - 3;
				while(path[i] != '/')
					i--;
				path[i+1] = '\0';
				continue;
			}
			if(watch == wd)
			{
//				strcpy(path,t);
				break;
			}
		}
	}
	if(feof(f))
		pos = -1;
	fclose(f);
//	fclose(j);
	return pos;
}

/*int main()
{
	char *path;
	path = (char*)malloc(1000);
	printf("%ld",search2(100,path));
}*/
