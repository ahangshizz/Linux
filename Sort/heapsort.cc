#include <iostream>
using namespace std;

void adjust(int* arr,int size,int root) {
  int child=2*root+1;
  while(child < size) {
    if(child+1 < size && arr[child] < arr[child+1]) {
      child+=1;
    }
    if(arr[child] > arr[root]) {
      swap(arr[child],arr[root]);
      root=child;
      child=2*root+1;
    }
    else {
      return; 
    }
  }
}

void heapsort(int* arr,int size) {
  int root=(size-1) >> 1;
  for(;root >= 0;--root)
    adjust(arr,size,root);
  int end=size-1;
  while(end) {
    swap(arr[0],arr[end]);
    adjust(arr,end,0);
    --end;
  }
}

int main() {
  int arr[]={2,5,1,3,6,4,8,7,9,0};
  for(int i=0;i < sizeof(arr)/sizeof(arr[0]);++i)
    cout<<arr[i]<<" ";
  cout<<endl;
  heapsort(arr,sizeof(arr)/sizeof(arr[0]));
  for(int i=0;i < sizeof(arr)/sizeof(arr[0]);++i)
    cout<<arr[i]<<" ";
  cout<<endl;
  return 0;
}
