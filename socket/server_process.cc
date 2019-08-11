#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SERV_PORT 9999

void do_handler(int num) {
  while(waitpid(0,NULL,WNOHANG)>0)
    ;
}

int main() {

  signal(SIGCHLD,do_handler);
  sockaddr_in servaddr , clieaddr;
  pid_t pid;
  socklen_t clielen=sizeof(clieaddr);
  int listenfd,connfd;
  char buf[BUFSIZ];
  char clieip[64];
  listenfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(SERV_PORT);
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr));
  listen(listenfd,256);
  printf("wait for client\n");
  while(1) {
    connfd=accept(listenfd,(sockaddr*)&clieaddr,&clielen);
    pid=fork();
    if(pid==0) {
      close(listenfd);
      while(1) {
        int n=0;
        n=read(connfd,buf,256);
        if(n==0) {
          printf("the other side has been closed\n");
          break;
        }
        printf("recevied from ip:%s at port %d\n",inet_ntop(AF_INET,&clieaddr.sin_addr,clieip,sizeof(clieip)),ntohs(clieaddr.sin_port));
        write(STDOUT_FILENO,buf,n);
        for(int i=0;i<n;++i) {
          buf[i]=toupper(buf[i]);
        }
        write(STDOUT_FILENO,buf,n);
        write(connfd,buf,n);
      }
      close(connfd);
      return 0;
    }else if(pid>0) {
      close(connfd);
    }else 
      perror("fork");
  }
  return 0;
}
