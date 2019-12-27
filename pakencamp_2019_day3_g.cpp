#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <cassert>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;
  int m;
  cin >> m;
  vector<ll> b(m);
  for (auto &x: b) cin >> x;
  int q;
  cin >> q;
  vector<vector<ll> > tcd(q, vector<ll>(3));
  for (auto &x: tcd) cin >> x[0] >> x[1] >> x[2];

  sort(a.begin(), a.end());
  if (m == 1) {
    ll t = a[n-1] - a[0];
    ll ans;
    if (b[0] < a[0]) ans = t + (a[0] - b[0]);
    else if (a[n-1] < b[0]) ans = t + (b[0] - a[n-1]);
    else ans = t + min(b[0] - a[0], a[n-1] - b[0]);
    cout << ans << endl;
  }

  return 0;
}
