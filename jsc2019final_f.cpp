// WIP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 998244353;

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

  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  for (auto &x: A) cin >> x;
  vector<int> L(Q), R(Q);
  for (int i = 0; i < Q; i++) {
    cin >> L[i] >> R[i];
  }

  vector<ll> factorial(N+2);
  factorial[0] = 1;
  for (int i = 1; i < N+2; i++) factorial[i] = (factorial[i-1] * i) % MOD;

  vector<vector<ll> > nPr(N+2, vector<ll>(N+2));
  for (int i = 0; i < N+2; i++) {
    for (int j = 0; j <= i; j++) {
      nPr[i][j] = (factorial[i] * modinv(factorial[i-j])) % MOD;
    }
  }

  vector<ll> ans(Q);
  for (int i = 0; i < Q; i++) {
    ll len = R[i] - L[i];
    ll factor = factorial[N-len];
    map<ll, int> count;
    for (int k = L[i]; k < R[i]; k++) {
      count[A[k]]++;
    }
    set<int> cc;
    for (auto &x: count) cc.insert(x.second);
    ans[i] = 1;
    for (auto it = cc.rbegin(); it != cc.rend(); it++) {
      auto &x = *it;
      ans[i] = (ans[i] * nPr[N-1][x]) % MOD;
    }
    ans[i] *= factor;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
