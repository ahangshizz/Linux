#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#define SERV_PORT 8888

int main() {
  int i,j,maxi,listenfd,connfd,sockfd;
  int nready;                             /*接收poll返回值,记录满足监听事件的fd个数*/
  ssize_t n;

  char buf[256],str[INET_ADDRSTRLEN];
  socklen_t clilen;
  struct pollfd client[1024];            /*创建poll参数的结构体*/
  struct sockaddr_in cliaddr,servaddr;
  listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  int opt=1;
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));  /*设置端口复用*/

  bzero(&servaddr,sizeof(servaddr));        /*清空客户端结构体*/
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

  bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr));/*为客户端绑定信息*/
  listen(listenfd,128);                /*设置监听的最大个数*/
  client[0].fd=listenfd;
  client[0].events=POLLIN;            /*要监听的第一个文件描述符,存入client[0],listenfd用来监听普通事件*/
  for(i=1;i<1024;++i) {
    client[i].fd=-1;                  /*把其他的client都初始化为-1*/
  }

  maxi=0;                             /*client中有效元素中最大元素下标*/
  for(;;) {
    nready=poll(client,maxi+1,-1);    /*阻塞监听是否有客服端链接请求*/
    if(client[0].revents & POLLIN) {
      clilen=sizeof(cliaddr);
      connfd=accept(listenfd,(sockaddr*)&cliaddr,&clilen);   /*接收客户端请求accept不会再进行阻塞*/
      printf("received from %s at PORT: %d \n",inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),ntohs(cliaddr.sin_port));
      for(i=1;i<1024;++i) {
        if(client[i].fd<0) {
          client[i].fd=connfd;        /*找到client[]中空闲的位置,存放accept返回的connfd*/
          break;
        }
      }
      if(i>maxi) {
        maxi=i;
      }
      if(--nready<=0) {
        continue;
      }

    }
    for(i=1;i<=maxi;++i) {
      if((sockfd=client[i].fd)<0) 
        continue;
      if(client[i].revents & POLLIN) {
        if((n=read(sockfd,buf,256))<0) {
          std::cout<<"error"<<std::endl;
        }else if(n==0) {
          printf("client[%d] closed connection\n",i);
          close (sockfd);
          client[i].fd=-1;
        } else {
          for(j=0;j<n;++j) {
            buf[j]=toupper(buf[j]);
            write(sockfd,buf,n);
          }
        }
        if(--nready<=0) {
          break;
        }


      }
    }
  }
  return 0;
}
