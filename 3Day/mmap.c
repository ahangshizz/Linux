/*************************************************************************
	> File Name: mmap.c
	> Author: 魏行
	> Mail: weihang2020@126.com
	> Created Time: 2019年05月27日 星期一 16时56分37秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
int main(int argc,const char* argv[])
{
	int fd=open("changhenge",O_RDWR);
	int len=lseek(fd,0,SEEK_END);
	void* ptr=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(ptr==MAP_FAILED)
	{
		perror("mmap error");
		return 0;
	}
	printf("%s",(char*)ptr);
	munmap(ptr,len);
	close(fd);
		return 0;	
}
