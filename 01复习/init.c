#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  pid_t pid1,pid2;
  pid1=fork();
  if(pid1==0) {
    pid2=fork();
    if(pid2>0)
      exit(0);
    sleep(2);
    printf("second child,parent pid=%ld\n",getppid());
  }
  waitpid(pid1,NULL,0);
  exit(0);
  return 0;
}
