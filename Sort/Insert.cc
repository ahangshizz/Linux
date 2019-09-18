#include <iostream>
#include <vector>
using namespace std;

void Insert(vector<int>& v) {
  int len=v.size();
  int j=0;
  for(int i=1;i < len;++i) {
    int temp=v[i];
    for(j=i-1;j>=0&&v[j]>temp;--j) {
      v[j+1]=v[j];
    }
    v[j+1]=temp;
  }
}

int main() {
  vector<int> v{1,0,3,2,5,4,6,8,9,7};
  Insert(v);
  for(auto e:v) {
    cout<<e<<" ";
  }
  cout<<endl;
  return 0;
}
