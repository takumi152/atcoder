#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

ll modinv(ll x, ll m = MOD) {
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

  int N, K;
  cin >> N >> K;

  // n-1Cr-1
  // -> K-1Ci-1
  // ボールがK個、箱が1<=i<=N-K+1個
  vector<ll> factorial(1, 1);
  for (int i = 1; i < 3 * N; i++) {
    factorial.push_back((factorial[i-1] * i) % MOD);
  }
  for (int i = 1; i <= K; i++) {
    if (i > N-K+1) {
      cout << 0 << endl;
      continue;
    }
    ll nCr = (((factorial[K-1] * modinv(factorial[K-i])) % MOD * modinv(factorial[i-1])) % MOD);
    ll boxcomb = (((factorial[N-K+1] * modinv(factorial[N-K+1-i])) % MOD * modinv(factorial[i])) % MOD);
    cout << (nCr * boxcomb) % MOD << endl;
  }

  return 0;
}
