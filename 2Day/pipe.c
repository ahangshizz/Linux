/*************************************************************************
	> File Name: pipe.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月22日 星期三 12时28分04秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main() {
	int fd[2];
	int ret=pipe(fd);
	if(ret==-1) {
		perror("pipe error");
		exit(1);
	}
	printf("ride fd[0]=%d\n",fd[0]);
	printf("write fd[0]=%d\n",fd[0]);
	close(fd[0]);
	close(fd[1]);
	return 0;
}
