#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

#define SERV_PORT 9999
#define OPEN_MAX 5000

int main() {
  int i=0;
  int socfd,lisfd,confd;
  int efd,nready,res;
  char buf[BUFSIZ];
  char clieip[16];
  struct sockaddr_in servaddr,clieaddr;
  struct epoll_event tep,ep[OPEN_MAX];
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  int opt=1;
  setsockopt(lisfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(lisfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(lisfd,64);
  efd=epoll_create(OPEN_MAX);
  tep.events=EPOLLIN;
  tep.data.fd=lisfd;
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  if(res==-1)
    perror("epoll_ctl");
  socklen_t clilen=sizeof(clieaddr);
  while(1) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    if(nready==-1)
      perror("epoll_wait");
    for(int i=0;i < nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==lisfd) {
        confd=accept(lisfd,(sockaddr*)&clieaddr,&clilen);  
        printf("ip:%s port:%d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,clieip,sizeof(clieip)),ntohs(clieaddr.sin_port));
        tep.data.fd=confd;
        tep.events=EPOLLIN;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,confd,&tep);
        if(res==-1)
          perror("epoll_ctl");
      }else {
        socfd=ep[i].data.fd;
        int len=read(socfd,buf,BUFSIZ);
        if(len < 0) {
          perror("read");
        }else if(len==0) {
          printf("the client:%d close\n",socfd);
          res=epoll_ctl(efd,EPOLL_CTL_DEL,socfd,NULL);
          close(socfd);
        }else if(len > 0) {
          write(STDOUT_FILENO,buf,len);
          for(int i=0;i < len;++i)
            buf[i]=toupper(buf[i]);
          write(STDOUT_FILENO,buf,len);
          write(socfd,buf,len);
        }
        
      }
    }
  }
  close(lisfd);
  close(efd);
  return 0;
}
