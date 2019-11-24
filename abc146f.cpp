#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  string s;
  cin >> s;

  reverse(s.begin(), s.end());

  int c = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] == '1') c++;
    else c = 0;
    if (c >= m) {
      cout << -1 << endl; // impossible
      return 0;
    }
  }

  int p = 0;
  vector<int> ans;
  while (p < n) {
    int maximum = 0;
    for (int i = p+1; i <= min(p+m, n); i++) {
      if (s[i] == '0') maximum = i - p;
    }
    ans.push_back(maximum);
    p += maximum;
  }

  reverse(ans.begin(), ans.end());

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
