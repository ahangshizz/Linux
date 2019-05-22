/*************************************************************************
	> File Name: pipe2.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月22日 星期三 14时19分51秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main() {
	int fd[2];
	int ret=pipe(fd);
	pid_t pid=fork();
	if(pid>0) {
	//输出重定向,父进程执行   ps  aux操作
	int STDOUT_FILEON;
	dup2(fd[1],STDOUT_FILEON);
	//关闭读操作
	close(fd[0]);
	execlp("ps","ps","aux",NULL);
	}
	else if(pid==0) {
		//输入重定向
		int STDIN_FILEON;
		dup2(fd[0],STDIN_FILEON);
		//关闭写操作
		close(fd[1]);
		execlp("grep","grep","weihang","--color=auto",NULL);
	}
	return 0;
}
