/*************************************************************************
	> File Name: alarm_uncle.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月28日 星期二 13时08分07秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() 
{
	alarm(1);
	int i=0;
	while(1) {
		printf("%d\n",i++);
	}
	return 0;
}
