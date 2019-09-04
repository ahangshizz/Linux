#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define OPEN_MAX 5000
#define SERV_PORT 9999

int main() {
  int sockfd,listfd,connfd;
  int res,efd,nready;
  char buf[BUFSIZ],ClieIp[16];
  socklen_t client;
  struct sockaddr_in servaddr,clieaddr;
  struct epoll_event ep[OPEN_MAX],tep;
  int opt=1;
  listfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  setsockopt(listfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  bind(listfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(listfd,256);
  efd=epoll_create(OPEN_MAX);
  if(efd==-1)
    perror("epoll_create");
  tep.data.fd=listfd,tep.events=EPOLLIN;
  res=epoll_ctl(efd,EPOLL_CTL_ADD,listfd,&tep);
  if(res==-1)
    perror("epoll_ctl");
  while(1) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);
    if(nready==-1)
      perror("epoll_wait");
    for(int i=0;i < nready;++i) {
      if(!(ep[i].events&EPOLLIN))
        continue;
      if(ep[i].data.fd==listfd) {
        client=sizeof(clieaddr);
        connfd=accept(listfd,(sockaddr*)&clieaddr,&client);
        char str[]="Welcome to connect\n";
        write(connfd,str,sizeof(str));
        printf("IP:%s PORT:%d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,ClieIp,sizeof(ClieIp)),ntohs(clieaddr.sin_port));
        tep.data.fd=connfd,tep.events=EPOLLIN;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,connfd,&tep);
        if(res==-1)
          perror("epoll_ctl");
      }else {
        sockfd=ep[i].data.fd;
        int len=read(sockfd,buf,BUFSIZ);
        if(len==0) {
          printf("The Client:%d Close\n",sockfd);
          res=epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);
          if(res==-1)
            perror("epoll_ctl");
        }else if(len < 0) {
          perror("read");
        }else {
          write(STDOUT_FILENO,buf,len);
          for(int j=0;j < len;++j)
            buf[j]=toupper(buf[j]);
          write(STDOUT_FILENO,buf,len);
          write(sockfd,buf,len);
        }
      }
    }
  }
  close(listfd);
  close(efd);
  
  return 0;
}
