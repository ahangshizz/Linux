#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include<stdlib.h>
#include <string.h>
#define SERV_PORT 9999

int main() {
  int i,len,maxi;
  int nready,client[FD_SETSIZE];
  int maxfd,sockfd,listenfd,connfd;
  char buf[BUFSIZ];
  char clieip[16];
  struct sockaddr_in servaddr,clieaddr;
  socklen_t clielen;
  fd_set rset,allset;
  listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  //端口复用
  int opt=1;
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(listenfd,128);
  maxfd=listenfd;
  maxi=-1;
  for(i=0;i < FD_SETSIZE;++i) {
    client[i]=-1;
  }
  FD_ZERO(&allset);
  FD_SET(listenfd,&allset);
  while(1) {
    rset=allset;
    nready=select(maxfd+1,&rset,NULL,NULL,NULL);
    if(nready < 0) {
      perror("select");
    }
    if(FD_ISSET(listenfd,&rset)) {
      clielen=sizeof(clieaddr);
      connfd=accept(listenfd,(sockaddr*)&clieaddr,&clielen);
      printf("clientip:%s port:%d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,clieip,sizeof(clieip)),ntohs(clieaddr.sin_port));
      for(i=0;i < FD_SETSIZE;++i) {
        if(client[i] < 0) {
          client[i]=connfd;
          break;
        }
      }
      if(i==FD_SETSIZE) {
        printf("too many cliens\n");
      }
      FD_SET(connfd,&allset);
      if(connfd > maxfd)
        maxfd=connfd;
      if(i > maxi)
        maxi=i;
      if(--nready==0)
        continue;
    }
    for(i=0;i < maxi;++i) {
      if((sockfd=client[i]) < 0)
        continue;
      if(FD_ISSET(sockfd,&rset)) {
        if((len=read(sockfd,buf,sizeof(buf)))==0) {
          close(sockfd);
          FD_CLR(sockfd,&allset);
          client[i]=-1;
        }else if(len > 0) {
          for(int j=0;j < len;++j) {
            buf[j]=toupper(buf[j]);
          }
          write(STDOUT_FILENO,buf,len);
          write(sockfd,buf,len);
        }
        if(--nready==0)
          break;
      }
    }
  }
  close(listenfd);


  return 0;
}
