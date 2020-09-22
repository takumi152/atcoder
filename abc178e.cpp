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

  int n;
  cin >> n;
  vector<pair<ll, ll> > xy(n);
  for (auto &x: xy) cin >> x.first >> x.second;

  for (auto &x: xy) {
    x = pair<ll, ll>(x.first + x.second, x.first - x.second);
  }

  ll x_min = (ll) 9e18;
  ll x_max = (ll) -9e18;
  ll y_min = (ll) 9e18;
  ll y_max = (ll) -9e18;
  for (auto &x: xy) {
    if (x.first < x_min) x_min = x.first;
    if (x.first > x_max) x_max = x.first;
    if (x.second < y_min) y_min = x.second;
    if (x.second > y_max) y_max = x.second;
  }

  cout << max(x_max - x_min, y_max - y_min) << endl;

  return 0;
}
