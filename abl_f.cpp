// AC-Library

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <cassert>

#include <atcoder/convolution>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

ll modinv(ll x, ll m = mod) {
  ll b = m;
  ll u = 1;
  ll v = 0;
  ll tmp;
  while (b) {
    ll t = x / b;
    x -= t * b;
    tmp = x;
    x = b;
    b = tmp;
    u -= t * v;
    tmp = u;
    u = v;
    v = tmp;
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> h(n*2);
  for (auto &x: h) cin >> x;

  unordered_map<int, ll> num_count;
  for (auto &x: h) num_count[x/10]++;

  vector<ll> a(8);
  for (auto &x: h) a[x/10]++;

  for (auto &x: a) cerr << x << " ";
  cerr << endl;

  vector<ll> c = atcoder::convolution<998244353>(a, a);

  for (int i = 0; i <= 7; i++) {
    c[i*2] = (c[i*2] - num_count[i] * num_count[i]) % mod;
    if (c[i*2] < 0) c[i*2] += mod;
  }
  for (int i = 0; i <= 14; i++) {
    c[i] = (c[i] * modinv(2)) % mod;
  }

  for (auto &x: c) cout << x << " ";
  cout << endl;

  return 0;
}
