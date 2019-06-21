#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  char* fifo="./test.fifo";
  umask(0);
  int ret=mkfifo(fifo,0664);
  if(ret<0) {
    perror("mkfifo");
    return -1;
  }
  printf("start open fifo\n");
  int fd=open(fifo,O_WRONLY);
  printf("end open fifo\n");
  if(fd<0) {
    perror("open");
    return -1;
  }
  printf("open fifo success\n");
  while(1) {
    char buf[1024]={0};
    printf("i say:");
    fflush(stdout);
    scanf("%s",buf);
    write(fd,buf,strlen(buf));
  }
  close(fd);
  return 0;
}
