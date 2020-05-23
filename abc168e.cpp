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

ll gcd(ll a, ll b) {
  while (b > 0) {
    ll rem = a % b;
    a = b;
    b = rem;
  }
  return a;
}

pair<ll, ll> reducedFraction(ll dividend, ll divisor) {
  if (dividend == 0) {
    if (divisor == 0) return pair<ll, ll>(0, 0);
    else return pair<ll, ll>(0, 1);
  }
  else if (divisor == 0) {
    return pair<ll, ll>(1, 0);
  }
  if (dividend < 0) {
    dividend = -dividend;
    divisor = -divisor;
  }
  int g = gcd(abs(dividend), abs(divisor));
  return pair<ll, ll>(dividend / g, divisor / g);
}

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
  vector<pair<ll, ll> > ab(n);
  for (auto &x: ab) cin >> x.first >> x.second;

  for (int i = 0; i < n; i++) ab[i] = reducedFraction(ab[i].first, ab[i].second);

  map<pair<ll, ll>, pair<ll, ll> > ratioCount;
  for (int i = 0; i < n; i++) {
    pair<ll, ll> complement = pair<ll, ll>(ab[i].second, -ab[i].first);
    if (complement.first < 0) complement = pair<ll, ll>(-complement.first, -complement.second);
    auto it = ratioCount.find(complement);
    if (it != ratioCount.end()) ratioCount[complement].second++;
    else ratioCount[ab[i]].first++;
  }

  ll zerozeroCount = 0;
  ll ans = 1;
  for (auto &x: ratioCount) {
    if (x.first == pair<ll, ll>(0LL, 0LL)) {
      zerozeroCount = x.second.first + x.second.second;
      continue;
    }
    ll c1 = 0;
    ll c2 = 0;
    if (x.second.first > 0) {
      c1 = modpow(2, x.second.first) - 1;
      if (c1 < 0) c1 += mod;
    }
    if (x.second.second > 0) {
      c2 = modpow(2, x.second.second) - 1;
      if (c2 < 0) c2 += mod;
    }
    ans = (ans * (1LL + c1 + c2)) % mod;
  }

  ans = (ans + zerozeroCount - 1) % mod;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
