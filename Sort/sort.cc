#include <iostream>

void Print(int* arr,int len) {
    for(int i=0;i < len;++i) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void QuickSort(int* arr,int begin,int end) {
    if(begin < end) {
        int i=begin,j=end;
        int tmp=arr[i];
        while(i < j) {
            while(i < j&&arr[j] >= tmp)--j;
            if(i < j)arr[i]=arr[j];
            while(i < j&&arr[i] <= tmp)++i;
            if(i < j)arr[j]=arr[i];
        }
        arr[i]=tmp;
        QuickSort(arr,begin,j-1);
        QuickSort(arr,i+1,end);
    }
}

void ShellSort(int* arr,int len) {
  int gap=len;
  int i,j,k;
  do{
    gap=gap/3+1;
    for(i = 0;i < gap;++i) {
      for(j = i+gap;j < len;j += gap) {
        int tmp=arr[j];
        for(k = j-gap;k >= 0 && arr[k] > tmp;k -= gap) 
          arr[k+gap]=arr[k];
        arr[k+gap]=tmp;
      }
    }
  }while(gap > 1);
}

void MergeArr(int* arr,int begin,int mid,int end,int* tmp);
void MergeSort(int* arr,int begin,int end,int* tmp) {
  if(begin < end) {
    int mid=(begin+end)/2;
    MergeSort(arr,begin,mid,tmp);
    MergeSort(arr,mid+1,end,tmp);
    MergeArr(arr,begin,mid,end,tmp);
  }
}
void MergeArr(int* arr,int begin,int mid,int end,int* tmp) {
  int i=begin,j=mid;
  int m=mid+1,n=end;
  int k=0;
  while(i <= j && m <= n) {
    if(arr[i] < arr[m])
      tmp[k++]=arr[i++];
    else 
      tmp[k++]=arr[m++];
  }
  while(i <= j) 
    tmp[k++]=arr[i++];
  while(m <= n) 
    tmp[k++]=arr[m++];
  for(i=0;i < k;++i){
    arr[begin+i]=tmp[i];
  } 
}

void AdJustDown(int* arr,int root,int len) {
  int child=root*2+1;
  while(child < len) {
    if(arr[child] < arr[child+1])
      child=child+1;
    if(arr[child] > arr[root]) {
      std::swap(arr[child],arr[root]);
    }
    root=child;
    child=root*2+1;
  }
}

void HeapSort(int* arr,int len) {
  int root=len/2;
  for(;root >= 0;--root) {
    AdJustDown(arr,root,len);
  }
  for(int i=len;i > 1;--i) {
    std::swap(arr[0],arr[i]);
    AdJustDown(arr,0,i-1);
  }
}


void BubbleSort(int* arr,int len) {
  for(int i=0;i < len-1;++i) {
    for(int j=0;j < len-1-i;++j) {
      if(arr[j]>arr[j+1])
        std::swap(arr[j],arr[j+1]);
    }
  }
}

int main() {
    int arr[]={1,5,4,3,2,6,8,9,7,0};
    int tmp[10]={0};
    Print(arr,sizeof(arr)/sizeof(int));
    //QuickSort(arr,0,9);
    //ShellSort(arr,10);
    //MergeSort(arr,0,9,tmp);
    //HeapSort(arr,9);
    BubbleSort(arr,10);
    Print(arr,sizeof(arr)/sizeof(int));
    return 0;
}
