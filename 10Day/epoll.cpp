#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#define SERV_PORT 8888
#define OPEN_MAX  1024
#define BUF_SIZE 256
using namespace std;
int main() {
  int i,listenfd,connfd,sockfd;
  int n,num=0;
  ssize_t nready,efd,res;
  char buf[BUF_SIZE],str[INET_ADDRSTRLEN];
  socklen_t clilen;
  struct sockaddr_in servaddr,cliaddr;
  struct epoll_event tep,ep[OPEN_MAX];       //tep:epoll_ctl参数  ep[] :epoo_wait 参数
  listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  int opt=1;
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));  //端口复用
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr));

  listen(listenfd,20);
  efd=epoll_create(OPEN_MAX);
  if(efd==-1) 
    cout<<"efd bad"<<endl;
  for(;;) {
    nready=epoll_wait(efd,ep,OPEN_MAX,-1);   //使用epoll为server端阻塞监听事件
    if(nready==-1) 
      cout<<"epoll_wait bad"<<endl;
    for(int i=0;i<nready;i++) {
      if(!(ep[i].events&EPOLLIN)) 
        continue;
      if(ep[i].data.fd==listenfd) {
        clilen=sizeof(cliaddr);
        connfd=accept(listenfd,(sockaddr*)&cliaddr,&clilen);
        printf("recevied from %s at PORE %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),ntohs(cliaddr.sin_port));
        tep.events=EPOLLIN;tep.data.fd=connfd;
        res=epoll_ctl(efd,EPOLL_CTL_ADD,connfd,&tep);
        if(res==-1) 
          cout<<"epoll_ctl error"<<endl;
      }else {
        sockfd=ep[i].data.fd;
        n=read(sockfd,buf,BUF_SIZE);
        if(n==0) {              //读到0说明这个客户端已经关闭连接
          res=epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);      //将这个套接字从红黑树中移除
          if(res==-1) 
            cout<<"epoll_ctl error"<<endl;
            close(sockfd);
            printf("client[%d] closed connection\n",sockfd);
          
        }else if(n<0) {
          cout<<"read error"<<endl;
          res=epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);
          close(sockfd);
        }else {
          for(i=0;i<n;++i) {
            buf[i]=toupper(buf[i]);
          }
          write(STDOUT_FILENO,buf,n);
          write(sockfd,buf,n);
        }
      }
    }
  }
  close(listenfd);
  close(efd);
  return 0;
}
