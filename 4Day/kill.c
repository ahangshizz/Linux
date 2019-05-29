/*************************************************************************
	> File Name: kill.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月28日 星期二 12时18分53秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	pid_t pid=fork();
	if(pid<0) {
		perror("fork");
	}
	if(pid>0) {
		while(1) {
			printf("i am you parent %d \n",getpid());
			sleep(1);
		}
	}
	if(pid==0) {
		sleep(2);
		kill(getppid(),SIGKILL);
	}
	return 0;

}
