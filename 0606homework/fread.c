#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main() 
{
  FILE* fp=fopen("test","r");
  if(!fp) {
    printf("fopen error\n");
  }
  char buf[1024];
  const char* msg="you had me at hello\n";
  while(1) {
    ssize_t s=fread(buf,1,strlen(msg),fp);
    if(s>0) {
      buf[s]=0;
      printf("%s\n",buf);
    }
    if(feof(fp)) {
      break;
    }
  }
  fclose(fp);
  return 0;
}
