#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>


#define SERV_PORT 9999
#define SERV_IP "172.17.10.1"

int main() {
  int sfd,cfd;
  struct sockaddr_in serv_addr,clien_addr;
  sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(SERV_PORT);

  bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

  listen(sfd,64);

  printf("wait for client connect\n");
  socklen_t clien_addr_len;
  clien_addr_len=sizeof(clien_addr);
  cfd=accept(sfd,(struct sockaddr*)&clien_addr,&clien_addr_len);
  char clien_ip[32];
  char buf[256];
  printf("client port:%d,client IP:%s",ntohs(clien_addr.sin_port),inet_ntop(AF_INET,&clien_addr.sin_addr.s_addr,clien_ip,sizeof(clien_ip)));
  while(1) {
    int len=read(cfd,buf,sizeof(buf));
    if(len>0) {
      write(STDOUT_FILENO,buf,len);
    }
    int i;
    for(i=0;i<len;++i) {
      buf[i]=toupper(buf[i]);
    }
    write(cfd,buf,len);
  }
  close (sfd);
  close (cfd);
  return 0;
}
