/*************************************************************************
	> File Name: execl.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月21日 星期二 20时01分39秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	int i=0;
	for(i=0;i<8;++i) {
		printf("parent i =%d \n",i);
	}
	pid_t pid=fork();
	if(pid==0) {
		execl("/bin/ls","www","-la",NULL);
	}
	for(i=0;i<3;++i) {
		printf("----------%d--------\n",i);
	}
	return 0;
}
