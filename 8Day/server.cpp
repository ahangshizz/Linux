#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#define SERV_PORT 8888

void wait_child(int signo) {
  while(waitpid(0,NULL,WNOHANG)>0);
  return;
}

int main() {
  int lfd,cfd;
  struct sockaddr_in serv_addr,clie_addr;
  lfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(SERV_PORT);
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  //inet_pton(AF_INET,"170.17.10.7,&serv_addr.sin_addr.s_addr");
  
  bind(lfd,(sockaddr*)&serv_addr,sizeof(serv_addr));
  listen(lfd,128);
  socklen_t clie_addr_len;
  clie_addr_len=sizeof(clie_addr);
  char clie_IP[256];
  while(1) {
  cfd=accept(lfd,(sockaddr*)&clie_addr,&clie_addr_len);
  printf("client IP:%s,port:%d\n",inet_ntop(AF_INET,&clie_addr.sin_addr.s_addr,clie_IP,sizeof(clie_IP)),ntohs(clie_addr.sin_port));
  pid_t pid;
  char buf[256];
  pid=fork();
  if(pid<0) {
    exit(1);
  }else if(pid==0) {
    close(lfd);
    while(1) {
      int n=read(cfd,buf,sizeof(buf));
      if(n==0) {
        close (cfd);
        return 0;
      }else if(n==-1) {
        perror("read error");
        exit(1);
      }else {
        for(int i=0;i<n;++i) {
          buf[i]=toupper(buf[i]);
        }
          write(cfd,buf,n);
          write(STDOUT_FILENO,buf,n);
      } 
    }
  }else {
      close(cfd);
      signal(SIGCHLD,wait_child);
  }
  }
  return 0;
}
