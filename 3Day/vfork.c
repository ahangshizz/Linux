/*************************************************************************
	> File Name: vfork.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月27日 星期一 19时29分49秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int main() 
{
	int pid=vfork();
	if(pid==0)
	{
		printf("i am child\n");
		sleep(5);
		exit(0);
		printf("i am two \n");
	}
	printf("i am parent\n");
}
