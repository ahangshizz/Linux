/*************************************************************************
	> File Name: time.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月29日 星期三 10时39分14秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

void dowork(int num) {
	//获取当前时间
	time_t curtime;
	time(&curtime);
	//格式化时间
	char* pt=ctime(&curtime);
	//将时间写入文件
	int O_CREAT;
	int O_APPEND;
	int O_WRONLY;

	int fd=open("/home/weihang/test.txt",O_CREAT | O_WRONLY | O_APPEND,0664);
	write(fd,pt,strlen(pt)+1);
	close(fd);
}

int main()
{
	pid_t pid=fork();
	if(pid>0) {
		//退出父进程
		exit(1);
	}else if(pid==0) {
		//变成会长
		setsid();
		//改变当前工作目录
		chdir("/home/weihang");
		//重设文件掩码
		umask(0);
		//关闭文件描述符
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		//执行核心操作
		//信号捕捉
		struct sigaction act;
		act.sa_flags =0;
		act.sa_handler=dowork;
		sigemptyset(&act);
		
		sigaction(SIGALRM,&act,NULL);
		struct itimerval val;
		//第一次触发时间
		val.it_value.tv_usec=0;
		val.it_value.tv_sec=2;
		//循环周期
		val.it_interval.tv_usec =0;
		val.it_interval.tv_sec =1;
		setitimer(ITIMER_REAL,&val,NULL);
		//保证子进程处于运行状态
		while(1);
	}
	return 0;
}
