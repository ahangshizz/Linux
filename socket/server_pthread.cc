#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>

#define SERV_PORT 9999
#define BUFSIZE 256
struct clilen {
  struct sockaddr_in cliaddr;
  int connfd;
};

void* do_work(void* arg) {
  int n,i;
  clilen* ts=(clilen*)arg;
  char buf[BUFSIZE];
  char str[BUFSIZE];
  while(1) {
    n=read(ts->connfd,buf,BUFSIZE);
    if(n==0) {
      printf("the client %d closed...\n",ts->connfd);
      break;
    }
    printf("received from %s at port %d\n",inet_ntop(AF_INET,&(*ts).cliaddr.sin_addr,str,sizeof(str)),ntohs(ts->cliaddr.sin_port));
    write(STDOUT_FILENO,buf,n);
    for(i=0;i<n;++i) {
      buf[i]=toupper(buf[i]);
    }
    write(STDOUT_FILENO,buf,n);
    write(ts->connfd,buf,n);
  }
  close(ts->connfd);
  return (void*)0;
}

int main() {
  int i=0;
  clilen ts[256];
  struct sockaddr_in servaddr,cliaddr;
  pthread_t tid;
  int listenfd,connfd;
  listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//创建一个socket,得到lfd
  
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  
  //绑定地址信息
  bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(listenfd,256);
  socklen_t clilen=sizeof(cliaddr);
  printf("wait for connect\n");

  while(1) {

    connfd=accept(listenfd,(sockaddr*)&cliaddr,&clilen);
    ts[i].cliaddr=cliaddr;
    ts[i].connfd=connfd;
    
    pthread_create(&tid,NULL,do_work,(void*)&ts[i]);
    pthread_detach(tid);
    ++i;
  }

  return 0;
}
