#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#define  SERV_PORT 9999
#define OPEN_MAX 5000

int main() {
  int i=0;
  int lisfd,confd,socfd;
  char buf[BUFSIZ],cliip[16];
  int efd,nready,res;
  struct sockaddr_in seraddr,cliaddr;
  struct epoll_event tep,ep[OPEN_MAX];
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  bzero(&seraddr,sizeof(seraddr));
  seraddr.sin_family=AF_INET;
  seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
  seraddr.sin_port=htons(SERV_PORT);

  bind(lisfd,(sockaddr*)&seraddr,sizeof(seraddr));
  listen(lisfd,256);
  //创建epoll模型
  efd=epoll_create(OPEN_MAX);
  tep.events=EPOLLIN;
  tep.data.fd=lisfd;
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  for(;;) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    for(i=0;i<nready;++i) {
      if(!(ep[i].events&EPOLLIN)) {
        continue;
      }
      if(ep[i].data.fd==lisfd) {
        socklen_t clien=sizeof(cliaddr);
        confd=accept(lisfd,(sockaddr*)&cliaddr,&clien);
        printf("clien ip:%s port:%d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,cliip,sizeof(cliip)),ntohs(cliaddr.sin_port));

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
          for(i=0;i<len;++i)
            buf[i]=toupper(buf[i]);
          write(STDOUT_FILENO,buf,len);
          write(socfd,buf,len);
        }
      }
    }
  }
  close (lisfd);
  close (efd);
  
  return 0;
}
