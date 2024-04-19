#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_set>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

ll modpow(ll a, ll b, ll m = mod) {
  ll r = 1;
  while (b > 0) {
    if (b & 1) r = (r * a) % m;
    a = (a * a) % m;
    b >>= 1;
  }
  return r;
}

ll modinv_universal(ll x, ll phi, ll m = mod) {
  return modpow(x, phi - 1, m);
}

ll euler_phi(ll n) {
  ll ret = n;
  for(ll i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      ret -= ret / i;
      while(n % i == 0) n /= i;
    }
  }
  if (n > 1) ret -= ret / n;
  return ret;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, a, b, c, x;
  cin >> n >> a >> b >> c >> x;

  if (b > c) swap(b, c);

  ll phi_b = euler_phi(b);
  ll phi_c = euler_phi(c);
  ll b_cycle = c / gcd(b, c);

  ll ans = 0;
  for (ll i = 1; i <= n; i++) {
    if (a * i > x) break; // impossible

    ll target = x - a * i;

    if (b == c) {
      if (target % b != 0) continue; // impossible
      ll b_plus_c = target / b;
      if (b_plus_c < 0 || b_plus_c > n * 2) continue; // impossible
      ans += min(b_plus_c + 1, n * 2 - b_plus_c + 1);
      continue;
    }

    // Bj + Ck == X - Ai を満たす (j, k) の個数は？
    if (target < c) break; // impossible

    ll bj_min = max(0LL, target - c * n);
    ll j_min = (bj_min + b - 1LL) / b;
    ll j_max = min(n, target / c);

    // Bj (mod C) == X - Ai (mod C) -> 適切なkを選べば Bj + Ck == X - Ai となる
    // j = (X - Ai) / B (mod C)
    ll j_first = ((target % c) * modinv_universal(b, phi_c, c)) % c;
    ll j_begin = j_first + max(0LL, b_cycle * ((j_min - j_first + b_cycle - 1) / b_cycle));
    if (j_begin > j_max) continue; // impossible
    cerr << i << " " << target << " " << j_max << " " << j_begin << " " << j_max - j_begin << " " << b_cycle << endl;
    ans += max(0LL, (j_max - j_begin + b_cycle - 1) / b_cycle);
  }

  cout << ans << endl;

  return 0;
}
