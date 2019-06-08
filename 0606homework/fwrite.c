#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main() 
{
  FILE* fp=fopen("test","wr");
  pid_t pid=fork();
  if(pid==0) {
    char buf[32]="you had me at hello\n";
    int i=0;
    for(i=0;i<5;++i) {
    fwrite(buf,strlen(buf),1,fp);
     }
    exit(0);
  }
  wait(NULL);
  fclose(fp);
  return 0;
}
