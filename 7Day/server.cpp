#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <ctype.h>
using namespace std;
#define SER_PORT 6666
int main() {
  int sfd,cfd;
  sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  struct sockaddr_in seraddr,cliaddr;
  seraddr.sin_family=AF_INET;
  seraddr.sin_port=htons(SER_PORT);
  seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
  bind(sfd,(sockaddr*)&seraddr,sizeof(seraddr));
  listen(sfd,128);
  socklen_t cliaddr_len;
  cliaddr_len=sizeof(cliaddr);
  cfd=accept(sfd,(sockaddr*)&cliaddr,&cliaddr_len);
  char buf[256];
  int len;
  while(1){
  len=read(cfd,buf,sizeof(buf));
  for(int i=0;i<len;++i) {
      buf[i]=toupper(buf[i]);
  }
  write(cfd,buf,len);
  }
  close(sfd);
  close(cfd);
  return 0;
}
