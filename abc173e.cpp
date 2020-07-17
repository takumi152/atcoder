#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  sort(a.rbegin(), a.rend(), [](auto &x, auto &y){return abs(x) < abs(y);});

  vector<ll> neg, pos;
  for (int i = 0; i < k; i++) {
    if (a[i] < 0) neg.push_back(a[i]);
    else if (a[i] > 0) pos.push_back(a[i]);
    else {
      cout << 0 << endl;
      return 0;
    }
  }
  if (neg.size() % 2 == 1) {
    ll next_neg = 0;
    bool next_neg_found = false;
    ll next_pos = 0;
    bool next_pos_found = false;
    for (int i = k; i < n; i++) {
      if (a[i] < 0) {
        next_neg = a[i];
        next_neg_found = true;
        break;
      }
    }
    for (int i = k; i < n; i++) {
      if (a[i] > 0) {
        next_pos = a[i];
        next_pos_found = true;
        break;
      }
    }
    if ((!next_neg_found && !next_pos_found) || (pos.size() == 0 && !next_pos_found)) {
      ll ans = 1;
      for (int i = n-1; i >= n - k; i--) {
        ans = (ans * a[i]) % mod;
        if (ans < 0) ans += mod;
      }
      cout << ans << endl;
      return 0;
    }
    else if (pos.size() == 0) {
      neg.pop_back();
      pos.push_back(next_pos);
    }
    else if (!next_neg_found) {
      neg.pop_back();
      pos.push_back(next_pos);
    }
    else if (!next_pos_found) {
      pos.pop_back();
      neg.push_back(next_neg);
    }
    else if (pos[pos.size()-1] * next_pos > neg[neg.size()-1] * next_neg) {
      neg.pop_back();
      pos.push_back(next_pos);
    }
    else {
      pos.pop_back();
      neg.push_back(next_neg);
    }
  }

  ll ans = 1;
  for (auto &x: pos) {
    ans = (ans * x) % mod;
  }
  for (auto &x: neg) {
    ans = (ans * x) % mod;
    if (ans < 0) ans += mod;
  }

  cout << ans << endl;

  return 0;
}
