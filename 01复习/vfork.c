#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int glov=6;
char buf[]="a write to stdout\n";

int main() {
  int var;
  pid_t pid;
  var=88;
  write(STDOUT_FILENO,buf,sizeof(buf)-1);
  printf("befor vfork\n");
  pid=vfork();
  if(pid==0) {
    ++var;
    ++glov;
    exit(0);
  }
  printf("pid=%d,glov=%d,var=%d\n",getpid(),glov,var);
  return 0;
}
