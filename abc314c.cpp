#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  string s;
  cin >> s;
  vector<int> c(n);
  for (auto &x: c) cin >> x;

  vector<vector<int>> color_index(m);
  for (int i = 0; i < n; i++) {
    color_index[c[i]-1].push_back(i);
  }

  string ans;
  vector<int> next_index(m, -1);
  for (int i = 0; i < n; i++) {
    ans.push_back(s[color_index[c[i]-1][(next_index[c[i]-1] + (int) color_index[c[i]-1].size()) % (int) color_index[c[i]-1].size()]]);
    next_index[c[i]-1]++;
  }

  cout << ans << endl;

  return 0;
}
