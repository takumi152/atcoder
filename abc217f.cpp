#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

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

  int n, m;
  cin >> n >> m;
  vector<vector<bool> > good(2 * n, vector<bool>(2 * n));
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    good[a-1][b-1] = true;
  }

  vector<ll> factorial(2 * n + 1);
  factorial[0] = 1;
  for (int i = 1; i <= 2 * n; i++) factorial[i] = (factorial[i-1] * i) % mod;

  vector<vector<ll> > ncr(2 * n + 1, vector<ll>(2 * n + 1));
  for (int i = 0; i <= 2 * n; i++) {
    for (int j = 0; j <= i; j++) {
      ncr[i][j] = (factorial[i] * modinv((factorial[i - j] * factorial[j]) % mod)) % mod;
    }
  }

  vector<vector<ll> > dp(2 * n + 1, vector<ll>(2 * n + 1, -1));
  auto solve = [&](auto self, int low, int high) {
    if (dp[low][high] != -1) return dp[low][high];

    if (high == low) dp[low][high] = 1;
    else {
      dp[low][high] = 0;
      for (int i = low + 2; i <= high; i++) {
        if (good[low][i-1]) {
          dp[low][high] = (dp[low][high] + (((self(self, low + 1, i - 1) * self(self, i, high)) % mod) * ncr[(high - low) / 2][(high - i) / 2])) % mod;
        }
      }
    }

    return dp[low][high];
  };

  solve(solve, 0, 2 * n);

  cout << dp[0][2 * n] << endl;

  return 0;
}
