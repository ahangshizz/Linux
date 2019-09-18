#include <iostream>
#include <vector>
using namespace std;

void Quick(vector<int>& v,int begin,int end) {
  int i=begin;
  int j=end;
  int tmp;
  if(i < j) {
    while(i < j) {
      tmp=v[i];
      while(i < j&&v[j] >= tmp) --j;
      if(i < j)v[i]=v[j];
      while(i < j&&v[i] <= tmp) ++i;
      if(i < j)v[j]=v[i];
    }
    v[i]=tmp;
    Quick(v,begin,j-1);
    Quick(v,i+1,end);
  }
}

int main() {
  vector<int> v{0,3,1,5,2,9,6,4,8,7};
  Quick(v,0,v.size()-1);
  for(auto e:v)
    cout<<e<<" ";
  cout<<endl;
  return 0;
}
