#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

ll modpow(ll a, ll b, ll m = mod) {
  ll r = 1;
  while (b > 0) {
    if (b & 1) r = (r * a) % m;
    a = (a * a) % m;
    b >>= 1;
  }
  return r;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string x;
  cin >> x;

  ll pcount = 0;
  for (int i = 0; i < n; i++) {
    if (x[i] == '1') pcount++;
  }

  vector<ll> ans(n);
  if (pcount == 0) {
    for (int i = 0; i < n; i++) ans[i] = 1;
  }
  else if (pcount == 1) {
    for (int i = 0; i < n; i++) {
      if (x[i] == '1') ans[i] = 0;
      else if (x[n-1] == '1' || i == n-1) ans[i] = 2;
      else ans[i] = 1;
    }
  }
  else {
    ll plus1 = 0;
    ll minus1 = 0;
    for (int i = 0; i < n; i++) {
      if (x[n-i-1] == '1') {
        plus1 = (plus1 + modpow(2, i, pcount+1)) % (pcount+1);
        minus1 = (minus1 + modpow(2, i, pcount-1)) % (pcount-1);
      }
    }
    for (int i = 0; i < n; i++) {
      ll v;
      if (x[i] == '0') v = (plus1 + modpow(2, n-i-1, pcount+1)) % (pcount+1);
      else {
        v = (minus1 - modpow(2, n-i-1, pcount-1)) % (pcount-1);
        if (v < 0) v += pcount-1;
      }
      ans[i] = 1;
      //cerr << i << " " << ans[i] << " " << v << endl;
      while (v > 0) {
        ll ppcount = 0;
        ll vt = v;
        while (vt > 0) {
          if (vt & 1) ppcount++;
          vt >>= 1;
        }
        v = v % ppcount;
        ans[i]++;
        //cerr << i << " " << ans[i] << " " << v << endl;
      }
    }
  }

  for (int i = 0; i < n; i++) cout << ans[i] << endl;

  return 0;
}
