/*************************************************************************
	> File Name: sigaction.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月29日 星期三 08时46分31秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void myfunc(int num) {
	printf("catch you signal\n");
	sleep(3);
	printf("wake up\n");
}

int main()
{
	struct sigaction act;
	act.sa_flags =0;
	sigemptyset(&act.sa_mask); 
	//添加屏蔽信号
	sigaddset(&act.sa_mask,SIGQUIT);
	act.sa_handler=myfunc;
	sigaction(SIGINT,&act,NULL);
	while(1) {
		printf("you had me at hello\n");
		sleep(1);
	}
	return 0;
}
