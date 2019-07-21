#include <iostream>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#define SERV_PORT 8888
class s_info {
  public:
  struct sockaddr_in clie_addr;
  int connfd;
};
void* work(void* arg) {
    int n,i;
    s_info *ts=(s_info*)arg;
    char buf[256];
    char str[16];
    while(1) {
      n=read(ts->connfd,buf,sizeof(buf));
      if(n==0) {
        printf("the client %d closed...\n",ts->connfd);
        break;
      }
      printf("receivd from %s at PORT %d\n",
          inet_ntop(AF_INET,&(*ts).clie_addr.sin_addr,str,sizeof(str)),ntohs((*ts).clie_addr.sin_port));
      for(i=0;i<n;++i) {
        buf[i]=toupper(buf[i]);
      }
      write(STDOUT_FILENO,buf,n);
      write(ts->connfd,buf,n);
    }
    close(ts->connfd);
} 
int main() {
  int lfd,connfd;
  s_info ts[256];
  int i=0;
  lfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  struct sockaddr_in serv_addr,clie_addr;
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(SERV_PORT);
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  socklen_t clien_addr_len;
  clien_addr_len=sizeof(clie_addr);
  bind(lfd,(sockaddr*)&serv_addr,sizeof(serv_addr));
  listen(lfd,128);
  while(1) {
    connfd=accept(lfd,(sockaddr*)&clie_addr,&clien_addr_len);
    ts[i].clie_addr=clie_addr;
    ts[i].connfd=connfd;
    pthread_t tid;
    pthread_create(&tid,NULL,work,(void*)&ts[i]);
    pthread_detach(tid);//线程分离,防止出现僵线程
    ++i;
  }
  return 0;
}
