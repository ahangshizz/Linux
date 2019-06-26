#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


pthread_mutex_t mutex;
int number=100;

void* Func(void* arg){
  while(1) {
    pthread_mutex_lock(&mutex);
    if(number>0) {
      usleep(1000);
      --number;
      printf("%d号黄牛抢到了票%d\n",(int)arg,number);
    }else {
      printf("已经没有票了,黄牛退出\n");
      //在任何线程有可能退出的地方都要解锁
      pthread_mutex_unlock(&mutex);
      pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex);
  }
}

int main() 
{
  pthread_mutex_init(&mutex,NULL);
  pthread_t tid[4];
  int i=0;
  for(i=0;i<4;++i) {
    pthread_create(&tid[i],NULL,Func,(void*)i);
  }
  for(i=0;i<4;++i) {
    pthread_join(tid[i],NULL);
  }
  pthread_mutex_destroy(&mutex);
  return 0;
}
