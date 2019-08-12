#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define OPEN_MAX 5000
#define SEV_PORT 9999
int main() {
  int i=0,j=0;
  int efd,res,nready;
  int lisfd,socfd,confd;
  char buf[BUFSIZ],clieip[16];
  struct epoll_event tep,ep[OPEN_MAX];
  struct sockaddr_in seraddr,cliaddr;
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  bzero(&seraddr,sizeof(seraddr));
  seraddr.sin_family=AF_INET;
  seraddr.sin_port=htons(SEV_PORT);
  seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
  int opt=1;
  setsockopt(lisfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  bind(lisfd,(sockaddr*)&seraddr,sizeof(seraddr));
  listen(lisfd,65);
  
  efd=epoll_create(OPEN_MAX);
  if(efd==-1)
    perror("epoll_create");
  tep.events=EPOLLIN;
  tep.data.fd=lisfd;
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  if(res==-1)
    perror("epoll_ctl");
  for(;;) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    if(nready==-1)
      perror("epoll_wait");
    for(i=0;i < nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==lisfd) {
        socklen_t client;
        confd=accept(lisfd,(sockaddr*)&cliaddr,&client);
        printf("IP:%s PORT:%d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,clieip,sizeof(clieip)),ntohs(cliaddr.sin_port));
        tep.events=EPOLLIN;
        tep.data.fd=confd;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,confd,&tep);
        if(res==-1)
          perror("epoll_ctl");
      }else {
        socfd=ep[i].data.fd;
        int len=read(socfd,buf,sizeof(buf));
        if(len==0) {
          printf("the client close\n");
          res=epoll_ctl(efd,EPOLL_CTL_DEL,socfd,NULL);
          close(socfd);
        }else if(len < 0) {
          perror("read");
        }else {

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
