/*************************************************************************
	> File Name: fork.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月21日 星期二 11时58分20秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc,const char* argv[]){
	pid_t pid;
	int i;
	for(i=0;i<4;++i) {
		printf("-------------i=%d\n",i);
	}
	pid=fork();
	if(pid>0) {
		printf("parent process=%d\n",getpid());
	}
	//子进程
	else if(pid==0) {
		printf("child process,pid=%d,ppid=%d\n",getpid(),getppid());
	}
	int j;
	for(j=0;j<4;++j) {
		printf("j=%d\n",j);
	}
	return 0;
}
