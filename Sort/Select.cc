#include <iostream>
#include <vector>
using namespace std;

void Select(vector<int>& v) {
  if(v.empty())
    return;
  for(int i=0;i < v.size();++i) {
    for(int j=i+1;j < v.size();++j) {
      if(v[i] > v[j])
        swap(v[i],v[j]);
    }
  }
}

int main() {
  vector<int> v{5,3,6,4,1,2,9,7,8,0};
  Select(v);
  for(auto e:v) {
    cout<<e<<" ";
  }
  cout<<endl;
  return 0;
}
