#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
  signal(SIGINT,SIG_IGN);
  while(1) {
    printf("you had me at hello\n");
    sleep(10);
  }
}
