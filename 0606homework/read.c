#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main() 
{
  int fd=open("./test",O_RDONLY);
  if(fd<0) {
    perror("open\n");
    return 1;
  }
  char* msg="you had me at hello\n";
  char buf[1024];
  while(1) {
    ssize_t s=read(fd,buf,strlen(msg));
    if(s>0) {
      printf("%s\n",buf);
    }else {
      break;
    }
  }
  close(fd);
  return 0;
}
