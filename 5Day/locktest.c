#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>
int number;
pthread_mutex_t mutex;
void * myfunc(void* arc) {
  int i=0;
  for(i=0;i<200;++i) {
    //加锁
    pthread_mutex_lock(&mutex);
    int cur=number;
    cur++;
    number=cur;
    printf("%d",number);
    //解锁
    pthread_mutex_unlock(&mutex);
    usleep(10);
  }
}
int main() {

  //释放锁资源
  pthread_mutex_destroy(&mutex);
  return 0;
}
