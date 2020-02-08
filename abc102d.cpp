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
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<ll> asum(n+1);
  for (int i = 0; i < n; i++) asum[i+1] = asum[i] + a[i];

  auto find_2part = [&](int a, int b) {
    int left = a;
    int right = b;
    while (left + 1 < right) {
      int center = (left + right) / 2;
      ll lsum = asum[center] - asum[a];
      ll rsum = asum[b] - asum[center];
      if (lsum == rsum) return center;
      else if (lsum < rsum) left = center;
      else right = center;
    }
    ll leftsum = asum[left] - asum[a];
    ll rightsum = asum[b] - asum[right];
    if (leftsum > rightsum) return left;
    else return right;
  };

  ll ans = inf;
  for (int i = 2; i < n-1; i++) {
    int lp = find_2part(0, i);
    int rp = find_2part(i, n);
    vector<ll> s = {asum[lp] - asum[0], asum[i] - asum[lp], asum[rp] - asum[i], asum[n] - asum[rp]};
    sort(s.begin(), s.end());
    ll diff = s[3] - s[0];
    if (diff < ans) ans = diff;
  }

  cout << ans << endl;

  return 0;
}
