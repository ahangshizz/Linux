#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void Print(char* str) {
  while(*str!='\0') {
    putc(*str,stdout);
    ++str;
  }
}

int main() {
  pid_t pid;
  pid=fork();
  if(pid==0) {
    Print("out of child\n");
  }
  if(pid>0) {
    Print("out of parent\n");
  }
  _exit(0);
}
