#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
int main() 
{
  int fd=open("text.txt",O_WRONLY | O_CREAT,0664);
  if(fd<0) {
    perror("open\n");
    return 0;
  }
  int i=0;
  char buf[]="you had me at hello\n";
  int len=strlen(buf);
  for(i=0;i<5;++i) {
    write(fd,buf,len);
  }
  close(fd);
  return 0;
}
