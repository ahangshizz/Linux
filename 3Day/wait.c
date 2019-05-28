/*************************************************************************
	> File Name: wait.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月27日 星期一 21时00分36秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
int main() 
{
	int pid =fork();
	if(pid<0)
	{
		perror("fork");
		exit(1);
	}
	else if(pid==0)
	{
		sleep(3);
		exit(0);
	}
	//wait 阻塞等待任意一个子进程的退出
	//wait(NULL);
	//waitpid阻塞等待子进程退出,可以指定子进程
	//waitpid(pid_t pid,int *status,int options);
	//pid==-1等待任意一个子进程,pid>0等待指定的子进程
	//options:WNOHANG:将waitpid设置为非阻塞    0   默认阻塞
	//返回值: ruguo WNOHANG 被指定 ,没有子进程退出立即报错返回0:  错误返回  -1;

	int statu;
	while(waitpid(pid,&statu,0)==0) {
		//非阻塞轮询操作
		printf("wait\n");
		sleep(1);
	}
	while(1)
	{
		printf("you had me at hello \n");
		sleep(1);
	}
	return 0;
}
