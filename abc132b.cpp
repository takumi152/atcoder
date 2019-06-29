#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> p;
  for (int i = 0; i < n; i++) {
    int buf;
    cin >> buf;
    p.push_back(buf);
  }

  int count = 0;
  for (int i = 1; i < p.size()-1; i++) {
    if ((p[i] > p[i-1] && p[i] < p[i+1]) || (p[i] < p[i-1] && p[i] > p[i+1])) count++;
  }

  cout << count << endl;
  return 0;
}
