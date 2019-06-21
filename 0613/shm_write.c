#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#define IPC_KEY 0x12345678
int main()
{
  int shmid=shmget(IPC_KEY,32,IPC_CREAT|0664);
  if(shmid<0) {
    perror("shmget");
    return -1;
  }
  void* shm_start=shmat(shmid,NULL,0);
  if(shm_start==(void*)-1){
    perror("shmat");
    return -1;
  }
  int i=0;
  while(1) {
    sprintf(shm_start,"%s-%d","明天很开心~~",i++);
    sleep(1);
  }
  shmdt(shm_start);
  shmctl(shmid,IPC_RMID,NULL);
  return 0;
}
