#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

void* myfunc(void* arg) {
  //打印子线程的ID
  printf("child thread id:%lu\n",pthread_self());
  return NULL;
}
int main() {
  //创建一个子进程
  //线程ID变量
  pthread_t pthid;
  pthread_create(&pthid,NULL,myfunc,NULL);
  printf("parent thread id %lu\n",pthread_self());
  int i=0;
  for(i=0;i<5;++i) {
    printf("i=%d\n",i);
  }
  sleep(2);
  return 0;
}
