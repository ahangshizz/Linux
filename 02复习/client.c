#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

#define SERV_PORT 9999
#define SERV_IP "172.17.10.7"

int main() {
  //定义套接字
  int sfd,len;
  struct sockaddr_in serv_addr;
  char buf[256];
  sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  //初始化结构体
  bzero(&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  inet_pton(AF_INET,SERV_IP,&serv_addr.sin_addr.s_addr);
  serv_addr.sin_port=htons(SERV_PORT);

  //连接服务器
  connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));


  while(1) {
    fgets(buf,sizeof(buf),stdin);

    write(sfd,buf,strlen(buf));

    len=read(sfd,buf,sizeof(buf));

    write(STDOUT_FILENO,buf,len);
  }
  return 0;
}
