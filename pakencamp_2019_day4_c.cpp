#include <iostream>
#include <iomanip>
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

  vector<pair<ll, ll> > lr(3);
  for (auto &x: lr) cin >> x.first >> x.second;

  ll hit = 0;
  ll base = 0;

  for (int i = lr[0].first; i <= lr[0].second; i++) {
    base += (lr[2].second - lr[2].first + 1) * (lr[1].second - lr[1].first + 1);
    hit += max(0L, min((lr[2].second - lr[2].first + 1), (lr[2].second - (i + 1) + 1))) * max(0L, min((lr[1].second - lr[1].first + 1), (lr[1].second - (i + 1) + 1)));
  }

  double ans = (double) hit / (double) base;

  cout << setprecision(16) << ans << endl;

  return 0;
}
