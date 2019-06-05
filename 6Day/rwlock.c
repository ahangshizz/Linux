#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

int number=0;
//创建读写锁
pthread_rwlock_t lock;
void* write_func(void* arg) {
  //循环写
  while(1) {
    pthread_rwlock_wrlock(&lock);
    number++;
    printf("==write:%lu,%d\n",pthread_self(),number);
    //解锁
    pthread_rwlock_unlock(&lock);
    sleep(1);
  }
  return NULL;
}
void* read_func(void* arg) {
  while(1) {
    //加读锁
    pthread_rwlock_rdlock(&lock);
    printf("==read:%lu,%d\n",pthread_self(),number);
    pthread_rwlock_unlock(&lock);
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_t p[8];
  pthread_rwlock_init(&lock,NULL);
  //循环创建三个写线程
  int i=0;
  for(i=0;i<3;++i) {
    pthread_create(&p[i],NULL,write_func,NULL);
  }
  //创建五个度线程
  int j=0;
  for(j=3;j<8;++j) {
    pthread_create(&p[j],NULL,read_func,NULL);
  }

  //阻塞回收子线程的pcb
  i=0;
  for(i=0;i<8;++i) {
    pthread_join(p[i],NULL);
  }
  //释放读写锁资源
  pthread_rwlock_destroy(&lock);
  return 0;
}
