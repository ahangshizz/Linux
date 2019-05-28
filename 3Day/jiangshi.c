/*************************************************************************
	> File Name: jiangshi.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月24日 星期五 09时15分34秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
int main() {
	//使用fork函数复制出一个子进程
	pid_t pid=fork();
	//如果子进程创建失败,直接打印函数错误,返回
	if(pid==-1) {
		printf("fork err\n");
		return 0;
	}
	//如果是子进程,让子进程直接死亡
	if(pid==0) {
		return 0;
	}
	//如果是父进程,让父进程死循环执行命令,没有时间去释放子进程的pcb
	if(pid>0) {
		while(1) {
			sleep(1);
			printf("------hello world------\n");
		}
	}
	return 0;
}
