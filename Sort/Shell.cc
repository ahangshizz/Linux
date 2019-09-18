#include <iostream>
#include <vector>
using namespace std;

void Shell(vector<int>& v) {
  int len=v.size();
  int gap=len;
  int i,j,k;
  do {
    gap=gap/3+1;
    for(i=0;i < gap;i++) {
      for(j=i+gap;j < len;j+=gap) {
        int tmp=v[j];
        for(k=j-gap;k >= 0&&v[k]>tmp;k-=gap) 
          v[k+gap]=v[k];
        v[k+gap]=tmp;
      }
    }


  }while(gap > 1);
}

int main() {
  vector<int> v{2,0,3,1,6,8,7,4,9};
  Shell(v);
  for(auto e:v) {
    cout<<e<<" ";
  }
  cout<<endl;
  return 0;
}
