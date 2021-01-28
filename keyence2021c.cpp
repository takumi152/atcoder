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

  int h, w, k;
  cin >> h >> w >> k;
  vector<vector<char> > board(h, vector<char>(w, '.'));
  for (int i = 0; i < k; i++) {
    int x, y;
    char c;
    cin >> x >> y >> c;
    board[x-1][y-1] = c;
  }

  vector<vector<ll> > dp(h+1, vector<ll>(w+1));
  dp[0][0] = modpow(3, h * w - k);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (board[i][j] == 'D') {
        dp[i+1][j] = (dp[i+1][j] + dp[i][j]) % mod;
      }
      else if (board[i][j] == 'R') {
        dp[i][j+1] = (dp[i][j+1] + dp[i][j]) % mod;
      }
      else if (board[i][j] == 'X') {
        dp[i+1][j] = (dp[i+1][j] + dp[i][j]) % mod;
        dp[i][j+1] = (dp[i][j+1] + dp[i][j]) % mod;
      }
      else {
        dp[i+1][j] = (dp[i+1][j] + ((dp[i][j] * modinv(3)) % mod) * 2) % mod;
        dp[i][j+1] = (dp[i][j+1] + ((dp[i][j] * modinv(3)) % mod) * 2) % mod;
      }
    }
  }

  cout << dp[h-1][w-1] << endl;

  return 0;
}
