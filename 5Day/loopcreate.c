#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* myfunc(void* arc) {
  //强制类型转换,把void*转换回int类型
  int num=(int)arc;
  printf("id:%d child pthread id:%lu\n",num,pthread_self());
}
int main() {
  //定义数组
  pthread_t pth[5]={0,1,2,3,4};
  int i=0;
  for(i=0;i<5;++i) {
    //为了能分辨出哪个线程是第几个创建的
    //所以把i传递给函数,让回调函数打印出来
    //由于cpu的时间碎片机制,对i进行传地址操作,会让序号混乱
    //所以选择强制类型转换,对i进行传值操作
    pthread_create(&pth[i],NULL,myfunc,(void*)i);
  }
  sleep(3);
  return 0;
}
