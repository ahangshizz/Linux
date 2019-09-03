#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/select.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define OPEN_MAX 5000
#define SERV_PORT 9999

int main() {
  int lisfd,confd,socfd;
  int i=0,j=0;
  int nready,efd,res;
  struct sockaddr_in servaddr,clieaddr;
  char buf[BUFSIZ]={0};
  socklen_t client;
  struct epoll_event tep,ep[OPEN_MAX];
  int opt=1;
  lisfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  setsockopt(lisfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(SERV_PORT);
  bind(lisfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(lisfd,256);
  efd=epoll_create(OPEN_MAX);
  tep.data.fd=lisfd,tep.events=EPOLLIN;
  res=epoll_ctl(efd,EPOLL_CTL_ADD,lisfd,&tep);
  if(res==-1)
    perror("epoll_ctl");
  char clieip[16];
  while(1) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    if(nready==-1)
      perror("epoll_wait");
    for(i=0;i < nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==lisfd) {
        client=sizeof(clieaddr);
        confd=accept(lisfd,(sockaddr*)&clieaddr,&client);
        printf("IP:%s PORT:%d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,clieip,sizeof(clieip)),ntohs(clieaddr.sin_port));
        tep.data.fd=confd;tep.events=EPOLLIN;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,confd,&tep);
        if(res==-1)
          perror("epoll_ctl");
      }
      else {
        socfd=ep[i].data.fd;
        int len=read(socfd,buf,BUFSIZ);
        if(len==0) {
          res=epoll_ctl(efd,EPOLL_CTL_DEL,socfd,NULL);
          if(res==-1)
            perror("epoll_ctl");
          close(socfd);
          printf("the client:%d closed\n",socfd);
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
