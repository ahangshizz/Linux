#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define SERV_PORT 9999
#define OPEN_MAX 5000

int main() {
  int i=0,j=0;
  int socfd,confd,lisfd;
  int efd,nready,res;
  char buf[BUFSIZ],clieip[16];
  struct sockaddr_in servaddr,clieaddr;
  struct epoll_event ep[OPEN_MAX],tep;
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  int opt=1;
  setsockopt(lisfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(lisfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(lisfd,65);
  tep.events=EPOLLIN,tep.data.fd=lisfd;
  efd=epoll_create(OPEN_MAX);
  if(efd==-1)
    perror("epoll_create");
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  if(res==-1)
    perror("epoll_ctl");
  socklen_t client;
  while(1) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    if(nready==-1)
      perror("epoll_wait");
    for(i=0;i < nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==lisfd) {
        socfd=accept(lisfd,(sockaddr*)&clieaddr,&client);
        printf("ip:%s port:%d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,clieip,sizeof(clieip)),ntohs(clieaddr.sin_port));
        tep.data.fd=socfd;
        tep.events=EPOLLIN;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,socfd,&tep);
        if(res==-1)
          perror("epoll_ctl");
      }else {
        confd=ep[i].data.fd;
        int len=read(confd,buf,BUFSIZ);
        if(len==0) {
          printf("the client:%d closed\n",confd);
          res=epoll_ctl(efd,EPOLL_CTL_DEL,confd,NULL);
        }else if(len < 0 ) {
          perror("read");
        } else {
          write(STDOUT_FILENO,buf,len);
          for(j=0;j < len;++j)
            buf[j]=toupper(buf[j]);
          write(STDOUT_FILENO,buf,len);
          write(confd,buf,len);
        }
      }
    }
  }
  close(lisfd);
  close(efd);
  return 0;
}
