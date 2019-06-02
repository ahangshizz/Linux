/*************************************************************************
	> File Name: minishell.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月30日 星期四 20时55分24秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
int main() 
{
	while(1) {
		printf("[wh@ahshizz ~]$ ");
    fflush(stdout);
		char tmp[1024];
		scanf("%[^\n]%*c",tmp);
		int argc=0;
		char* argv[32]={NULL};
		char* ptr=tmp;
		while(*ptr!='\0') {
			if(!isspace(*ptr)) {
				argv[argc]=ptr;
        argc++;
				while(!isspace(*ptr)&&*ptr!='\0') {
					ptr++;
				}
        *ptr='\0';
        ptr++;
        continue;
			}
      ptr++;
		}
    argv[argc]=NULL;
    if(!strcmp(argv[0],"cd")) {
      chdir(argv[1]);
      continue;
    }
    pid_t pid=fork();
    if(pid==0) {
      execvp(argv[0],argv);
      exit(0);
    }
    wait(NULL);
	}
	return 0;
}
