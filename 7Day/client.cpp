#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
using namespace std;
#define SERV_PORT 6666
#define SERV_IP "172.17.10.7"
int main() {
  int cfd;
  cfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in serv_addr;
  socklen_t serv_addr_len;
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(SERV_PORT);
  inet_pton(AF_INET,SERV_IP,&serv_addr.sin_addr.s_addr);
  connect(cfd,(sockaddr*)&serv_addr,sizeof(serv_addr));
  char buf[256]; 
  while(1) {
  fgets(buf,sizeof(buf),stdin);
  write(cfd,buf,strlen(buf));
  int len=read(cfd,buf,sizeof(buf));
  write(STDOUT_FILENO,buf,len);
  }
  close(cfd);
  return 0;
}
