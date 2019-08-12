#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/epoll.h>
#include <stdio.h>
#define SERV_PORT 9999
#define OPEN_MAX 5000
#include <arpa/inet.h>


int main() {
  int i=0;
  int lisfd,confd,socfd;
  int efd,res,nready;
  struct sockaddr_in seraddr,cliaddr;
  struct epoll_event tep,ep[OPEN_MAX];
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  char buf[BUFSIZ];
  bzero(&seraddr,sizeof(seraddr));
  int opt=1;
  setsockopt(lisfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  seraddr.sin_family=AF_INET;
  seraddr.sin_port=htons(SERV_PORT);
  seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(lisfd,(sockaddr*)&seraddr,sizeof(seraddr));
  listen(lisfd,128);
  efd=epoll_create(OPEN_MAX);
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  for(;;) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    
    for(i=0;i<nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==lisfd) {
        socklen_t clen;
        confd=accept(lisfd,(sockaddr*)&cliaddr,&clen);
        tep.events=EPOLLIN;
        tep.data.fd=confd;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,confd,&tep);

      }else {
        socfd=ep[i].data.fd;
        int len=read(socfd,buf,BUFSIZ);
        if(len==0) {
          res=epoll_ctl(efd,EPOLL_CTL_DEL,socfd,NULL);
          close(socfd);
        }else {
          write(STDOUT_FILENO,buf,len);
          for(i=0;i<len;++i) {
            buf[i]=toupper(buf[i]);
          }
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
