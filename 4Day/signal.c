/*************************************************************************
	> File Name: signal.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月29日 星期三 08时33分32秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>



void myfunc(int num)
{
	printf("catch you signal:%d\n",num);
}
int main()
{
	//捕捉Ctrl+c
	//注册捕捉函数
	signal(SIGINT,myfunc);
	while(1) {
		printf("you had me at hello \n");
		sleep(1);
	}
	return 0;
}
