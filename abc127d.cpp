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
  vector<ll> a(n);
  for (auto &x: a) cin >> x;
  vector<pair<int, ll> > cb(m);
  for (int i = 0; i < m; i++) cin >> cb[i].second >> cb[i].first;

  sort(a.begin(), a.end());
  sort(cb.rbegin(), cb.rend());

  int p = 0;
  int q = 0;
  int r = cb[0].second;
  while (true) {
    if (a[p] < cb[q].first) {
      a[p] = cb[q].first;
      p++;
      if (p >= n) break;
      r--;
      if (r <= 0) {
        q++;
        if (q >= m) break;
        r = cb[q].second;
      }
    }
    else break;
  }

  ll ans = 0;
  for (auto &x: a) ans += x;

  cout << ans << endl;

  return 0;
}
