#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include<utmp.h>
#include<malloc.h>
#include<stdlib.h>
#include<sys/inotify.h>

#ifndef _INITDB_H
#define _INITDB_H

char list[][15] = {"root","home","bin","sbin","lib","etc","usr","srv","var","boot","tmp","media","mnt","misc","net","opt","lost+found"};
FILE *fil;
long count;

void user_logged(char*);
void initiate();
void buildDB();
void recursive_call(char*);

#endif
