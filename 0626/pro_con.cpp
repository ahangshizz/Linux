//生产者和消费者模型
//使用队列存储生产的数据
#include <iostream>
#include <pthread.h>
#include <queue>
#define MAX_CAP 10
using namespace std;
class BlockQueue
{
  private:
    queue<int> _queue;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_pro;
    pthread_cond_t _cond_con;
  public:
    BlockQueue(int cap=MAX_CAP) {
      this->_capacity=cap;
      pthread_mutex_init(&_mutex,NULL);
      pthread_cond_init(&_cond_pro,NULL);
      pthread_cond_init(&_cond_con,NULL);
    }
    ~BlockQueue() {
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond_pro);
      pthread_cond_destroy(&_cond_con);
    }
    bool QueuePush(int data) {
      pthread_mutex_lock(&_mutex);
      //一定要使用while循环判断条件是否成立
      while(_queue.size()==_capacity) {
        pthread_cond_wait(&_cond_pro,&_mutex);
      }
      _queue.push(data);
      pthread_cond_signal(&_cond_con);
      pthread_mutex_unlock(&_mutex);
    }
    bool QueuePop(int &data) {
      pthread_mutex_lock(&_mutex);
      while(_queue.empty()) {
        pthread_cond_wait(&_cond_con,&_mutex);
      }
      data=_queue.front();
      _queue.pop();
      pthread_cond_signal(&_cond_pro);
      pthread_mutex_unlock(&_mutex);
    }
};
//生产数据
void* Product(void* arg) {
  BlockQueue* q=(BlockQueue*)arg;
  int i=0;
  while(1) {
      q->QueuePush(i++);
      cout<<"product:"<<pthread_self()<<"put data"<<i<<endl;
  }
  return NULL;
}
//消费数据
void* Consumer(void* arg) {
  BlockQueue* q=(BlockQueue*)arg;
  while(1) {
    int data;
    q->QueuePop(data);
    cout<<"consumer:"<<pthread_self()<<"get data "<<data<<endl;
  }
  return NULL;
}
int main()
{
  pthread_t con_tid[4],pro_tid[4];
  BlockQueue q;
  for(int i=0;i<4;++i) {
    int ret=pthread_create(&con_tid[i],NULL,Consumer,(void*)&q);
    if(ret!=0) {
      return -1;
    }
  }
  for(int i=0;i<4;++i) {
    int ret=pthread_create(&pro_tid[i],NULL,Product,(void*)&q);
    if(ret!=0) {
      return -1;
    }
  }
  for(int i=0;i<4;++i) {
    pthread_join(con_tid[i],NULL);
  }
  for(int i=0;i<4;++i) {
    pthread_join(pro_tid[i],NULL);
  }
  return 0;
}
