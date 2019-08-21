#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#define SERV_PORT 9999
#define OPEN_MAX 5000

int main() {
  int i=0,j=0;
  int socfd,confd,lisfd;
  int efd,nready,res;
  struct sockaddr_in servaddr,clieaddr;
  struct epoll_event ep[OPEN_MAX],tep;
  char buf[BUFSIZ],clieip[16];
  socklen_t client;
  int opt=1;
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  setsockopt(lisfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  bind(lisfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(lisfd,OPEN_MAX);
  efd=epoll_create(OPEN_MAX);
  tep.data.fd=lisfd;
  tep.events=EPOLLIN;
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  if(res==-1)
    perror("epoll_ctl");
  printf("wait for connect\n");
  while(1) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    if(nready==-1)
      perror("epoll_wait");
    for(i=0;i < nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==lisfd) {
        confd=accept(lisfd,(sockaddr*)&clieaddr,&client);
        write(confd,"welcome connect",16);
        printf("IP:%s PORT:%d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,clieip,sizeof(clieip)),ntohs(clieaddr.sin_port));
        tep.data.fd=confd;
        tep.events=EPOLLIN;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,confd,&tep);
        if(res==-1)
          perror("epoll_ctl");
      }else {
        socfd=ep[i].data.fd;
        int len=read(socfd,buf,BUFSIZ);
        if(len==0) {
          printf("the client:%d closed\n",socfd);
          res=epoll_ctl(efd,EPOLL_CTL_DEL,confd,NULL);
          if(res==-1)
            perror("epoll_ctl");
        }else if(len < 0) {
          perror("read");
        }else {
          write(STDOUT_FILENO,buf,len);
          for(j=0;j < len;++j)
            buf[j]=toupper(buf[j]);
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
