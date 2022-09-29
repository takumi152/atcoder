#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string s;
  cin >> s;

  list<int> res;
  res.push_back(0);
  auto it = res.begin();
  for (int i = 0; i < n; i++) {
    if (s[i] == 'R') it++;
    it = res.insert(it, i+1);
  }

  for (auto &x: res) cout << x << " ";
  cout << endl;

  return 0;
}
