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

  ll n, m;
  cin >> n >> m;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  sort(a.begin(), a.end());

  vector<ll> asum(n+1);
  for (int i = 0; i < n; i++) {
    asum[i+1] = asum[i] + a[i];
  }

  ll left = 0;
  ll right = 200000;
  while (left < right) {
    ll center = (left + right + 1) / 2;
    ll count = 0;
    for (int i = 0; i < n; i++) {
      ll target = center - a[i];
      auto lb = lower_bound(a.begin(), a.end(), target);
      ll d = distance(lb, a.end());
      count += d;
    }
    if (count >= m) left = center;
    else right = center - 1;
  }

  ll count = 0;
  ll score = 0;
  for (int i = 0; i < n; i++) {
    ll target = left - a[i];
    auto lb = lower_bound(a.begin(), a.end(), target);
    ll d = distance(lb, a.end());
    count += d;
    score += (asum[n] - asum[n-d]) + (a[i] * d);
  }
  score -= left * (count - m);

  cout << score << endl;

  return 0;
}
