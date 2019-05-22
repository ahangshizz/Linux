/*************************************************************************
	> File Name: execltest.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月22日 星期三 08时03分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>


int main() {
	pid_t pid;
	pid=fork();
	if(pid==0) {
		while(1) {
			sleep(1);
			printf("child poress\n");
		}
		
	}


	return 0;
}
