#include <iostream>
#include <vector>
using namespace std;

void Bubble(vector<int>& v) {
  if(v.empty())
    return;
  for(int i=0;i < v.size()-1;++i) {
    for(int j=0; j < v.size()-i-1;++j) {
      if(v[j]>v[j+1])
        swap(v[j],v[j+1]);
    }
  }
}

int main() {
  vector<int> v{2,5,7,6,8,2,4,9,7,9,6,4,3};
  Bubble(v);
  for(auto e:v)  {
    cout<<e<<" ";
  }
  return 0;
}
