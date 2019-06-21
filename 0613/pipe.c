#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main()
{
  int fd[2];
  int ret=pipe(fd);
  pid_t pid=fork();
  if(pid==0) {
    dup2(fd[1],1);
    close(fd[0]);
    execlp("ps","ps","aux",NULL);
  }
  else if(pid>0) {
    dup2(fd[0],0);
    close(fd[1]);
    execlp("grep","grep","pipe","--color=auto",NULL);
  }
  return 0;
}
