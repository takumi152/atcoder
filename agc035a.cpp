#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> a(N);
  for (auto &x: a) cin >> x;

  map<int, int> bucket;
  for (auto &x: a) bucket[x]++;

  if (bucket.size() == 1) {
    int num = 0, count = 0;
    for (auto &x: bucket) num = x.first, count = x.second;
    if (num == 0) cout << "Yes" << endl;
    else cout << "No" << endl;
  }
  else if (bucket.size() == 2) {
    vector<int> num, count;
    for (auto x: bucket) {
      num.push_back(x.first);
      count.push_back(x.second);
    }
    if (num[0] != 0) cout << "No" << endl;
    else if (count[0] * 2 >= count[1] && count[1] % 2 == 0) cout << "Yes" << endl;
    else cout << "No" << endl;
  }
  else if (bucket.size() == 3) {
    vector<int> num, count;
    for (auto x: bucket) {
      num.push_back(x.first);
      count.push_back(x.second);
    }
    if (count[0] != count[1] || count[1] != count[2]) cout << "No" << endl;
    else if ((num[0] ^ num[1]) == num[2]) cout << "Yes" << endl;
    else cout << "No" << endl;
  }
  else cout << "No" << endl;

  return 0;
}
