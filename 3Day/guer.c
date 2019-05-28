/*************************************************************************
	> File Name: guer.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月24日 星期五 09时08分05秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main() {
	pid_t pid;
	pid=fork();
	//如果子进程创建失败,直接返回,并打印fork函数错误
	if(pid==-1) {
		printf("fork err\n");	
		return 0;
	}
	//如果是子进程让子进程死循环执行命令
	if(pid==0) {
		while(1) {
			sleep(1);
			printf("hello world\n");
		}
	}
	//父进程直接死亡
	if(pid>0) {
		return 0;
	}
	return 0;
}
