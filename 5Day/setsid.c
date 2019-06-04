/*************************************************************************
	> File Name: setsid.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月29日 星期三 10时18分24秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
int main() 
{
	//创建一个会话
	//将子进程变成会长
	pid_t pid=fork();
	if(pid>0) {
		exit(1);
#if 0
		kill(getpid(),SIGKILL);
		raise(SIGKILL);
#endif
	}
	else if(pid==0) {
		//变成会长
		setsid();
		//让子进程一直活着
		while(1); 
	}
	
	return 0;
}
