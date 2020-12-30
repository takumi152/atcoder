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
  ll x;
  cin >> n >> x;
  vector<ll> a(n);
  for (auto &y: a) cin >> y;

  reverse(a.begin(), a.end());

  if (x % a[0] == 0) {
    cout << 1 << endl;
    return 0;
  }
  ll pos = 1;
  ll neg = 1;
  ll rem_pos = x % a[0];
  ll rem_neg = -(x % a[0] - a[0]);
  for (int i = 1; i < n; i++) {
    if (rem_pos % a[i] == 0) break;
    ll next_pos = 0;
    ll next_neg = 0;
    if (a[i] * (rem_pos / a[i] + 1) == a[i-1]) next_pos += pos;
    else {
      next_pos += pos;
      next_neg += pos;
    }
    if (a[i] * (rem_neg / a[i] + 1) == a[i-1]) next_neg += neg;
    else {
      next_neg += neg;
      next_pos += neg;
    }
    rem_pos = rem_pos % a[i];
    rem_neg = -(rem_pos % a[i] - a[i]);
    pos = next_pos;
    neg = next_neg;
  }

  ll ans = pos + neg;

  cout << ans << endl;


  return 0;
}
