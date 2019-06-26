#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
void* Func(void* arg) {
  while(1) {
    printf("i am created pthread------\n");
    sleep(1);
  }
  return NULL;
}
int main()
{
  pthread_t tid;
  char* param="this is input param";
  int ret=pthread_create(&tid,NULL,Func,(void*)param);
  if(ret!=0) {
    printf("pthread create error\n");
    return -1;
  }
  printf("tid:%d\n",tid);
  while(1) {
    printf("i an main pthread----\n");
    sleep(1);
  }
  return 0;
}
