#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
  char* fifo="./test.fifo";
  //umask(0);
  //int ret=mkfifo(fifo,0664);
  //if(ret<0) {
   // perror("mkfifo");
   // return -1;
  //}
  printf("start open fifo\n");
  int fd=open(fifo,O_RDONLY);
  printf("end open fifo\n");
  if(fd<0) {
    perror("open fifo");
    return -1;
  }
  printf("fifo:%d open fifo success\n",fifo);
  while(1) {
    sleep(5);
    char buf[1024]={0};
    read(fd,buf,1023);
    printf("peer say:%s\n",buf);
  }
  close(fd);
  return 0;
}
