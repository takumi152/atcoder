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

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  vector<Pii> ans;
  for (int k = 1; k <= n - 2; k++) {
    for (int i = 0; i < n; i++) {
      if (p[i] == k) {
        if (i == n - 1) {
          int p1 = p[i-1];
          int p2 = p[i];
          p.erase(p.begin() + i - 1);
          p.erase(p.begin() + i - 1);
          p.insert(p.begin() + i - 2, p2);
          p.insert(p.begin() + i - 2, p1);
          ans.emplace_back(i, i - 2);
          i = n - 2;
        }
        int p1 = p[i];
        int p2 = p[i+1];
        p.erase(p.begin() + i);
        p.erase(p.begin() + i);
        p.insert(p.begin() + k - 1, p2);
        p.insert(p.begin() + k - 1, p1);
        ans.emplace_back(i + 1, k - 1);
      }
    }
  }
  if (p[n - 2] == n - 1) {
    cout << "Yes" << endl;
    cout << ans.size() << endl;
    for (auto &x: ans) cout << x.first << " " << x.second << endl;
  }
  else {
    cout << "No" << endl;
  }

  return 0;
}
