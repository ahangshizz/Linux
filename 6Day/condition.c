#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
//创建结点结构
typedef struct node {
  int data;
  struct node* next;
}node;
//定义指向链表头部的指针
node* head=NULL;
//线程同步---互斥锁
pthread_mutex_t mutex;
//阻塞线程
pthread_cond_t cond;
//生产者
void* wr_func(void* arg) {
  while(1) {
    //创建一个链表的结点
    node* pnew=(node*)malloc(sizeof(node));
    //节点的初始化
    pnew->data=rand()%1000;
    //使用互斥锁保护共享资源
    pthread_mutex_lock(&mutex);
    //指针域
    pnew->next=head;
    head=pnew;
    printf("=====生产:%lu,%d\n",pthread_self(),pnew->data);
    pthread_mutex_unlock(&mutex);
    //通知阻塞的线程解除阻塞
    pthread_cond_signal(&cond);
    sleep(rand()%3);
  }
  return NULL;
}
void* rd_func(void* arg) {
  while(1) {
    pthread_mutex_lock(&mutex);
    //判断链表是否为空
    if(head==NULL) {
      //线程阻塞
      //该函数会对互斥锁解锁
      pthread_cond_wait(&cond,&mutex);
      //解除阻塞后,对互斥锁做加锁操作
    }
    //链表不为空
    //产生消费行为,删除头结点
    node* Pple=head;
    head=head->next;
    printf("-----消费者: %lu,%d\n",pthread_self(),Pple->data);
    free(Pple);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
int main()
{
  pthread_t p1,p2;
  //初始化
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond,NULL);
  //创建生产者线程
  pthread_create(&p1,NULL,wr_func,NULL);
  //创建消费者线程
  pthread_create(&p2,NULL,rd_func,NULL);

  //阻塞回收线程
  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
