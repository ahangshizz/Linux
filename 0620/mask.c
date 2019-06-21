#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//捕捉信号以后替换的函数
void sigcb(int signo) {
  printf("recv signo:%d\n",signo);
}
int main()
{
  //信号捕捉
  signal(SIGINT,sigcb);
  signal(40,sigcb);
  sigset_t set,oldset;
  sigemptyset(&set);
  //int sigfillset(sigset_t* set)
  sigfillset(&set);
  sigprocmask(SIG_BLOCK,&set,&oldset);

  getchar();
  sigprocmask(SIG_UNBLOCK,&set,NULL);
  //sigprocmask(SIG_SETMASK,&oldset,NULL);
  
  return 0;
}
