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

  int k, q;
  cin >> k >> q;
  vector<ll> d(k);
  for (auto &y: d) cin >> y;
  vector<ll> n(q), x(q), m(q);
  for (int i = 0; i < q; i++) cin >> n[i] >> x[i] >> m[i];

  vector<ll> ans(q);
  for (int i = 0; i < q; i++) {
    vector<ll> d_mod(k);
    for (int j = 0; j < k; j++) {
      d_mod[j] = d[j] % m[i];
      if (d_mod[j] == 0) d_mod[j] = m[i];
    }
    vector<ll> d_sum(k+1);
    for (int i = 0; i < k; i++) d_sum[i+1] = d_sum[i] + d_mod[i];
    ll total = (x[i] % m[i]) + (d_sum[k] * ((n[i]-1) / k)) + d_sum[(n[i]-1)%k];
    ans[i] = (n[i] - 1) - (total / m[i]);
  }

  for (auto &y: ans) cout << y << endl;

  return 0;
}
