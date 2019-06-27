//厨师做面
//顾客吃面
//四个厨师,四个顾客,使用条件变量加互斥锁进行同步互斥
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int have_noodle=0;
pthread_mutex_t mutex;
pthread_cond_t cond_eat;
pthread_cond_t cond_cook;

//顾客吃面
void* eat_noodle(void* arg) {
  while(1) {
    sleep(1);
    pthread_mutex_lock(&mutex);
    while(have_noodle==0) {
      pthread_cond_wait(&cond_eat,&mutex);
    }
    have_noodle=0;
    printf("%d eat a noodle\n",(int)arg);
    pthread_cond_signal(&cond_cook);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
//厨师做面
void* cook_noodle(void* arg) {
  while(1) {
    sleep(1);
    pthread_mutex_lock(&mutex);
    //使用while循环判断条件是否成立
    while(have_noodle==1) {
      pthread_cond_wait(&cond_cook,&mutex);
    }
    have_noodle=1;
    printf("%d cook a noodle\n",(int)arg);
    pthread_cond_signal(&cond_eat);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
int main() 
{
  pthread_t tid1,tid2;
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond_eat,NULL);
  pthread_cond_init(&cond_cook,NULL);
  int i=0;
  for(i=0;i<4;++i) {
    int ret=pthread_create(&tid1,NULL,eat_noodle,(void*)i);
    if(ret!=0) {
      return -1;
    }
  }
  for(i=0;i<4;++i) {
    int ret=pthread_create(&tid2,NULL,cook_noodle,(void*)i);
    if(ret!=0) {
      return -1;
    }
  }
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond_cook);
  pthread_cond_destroy(&cond_eat);
  return 0;
}
