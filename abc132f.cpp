// WIP
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

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

  int n, k;
  cin >> n >> k;

  vector<vector<ll> > dp(1, vector<ll>(((int) sqrt(n))+1, 1));
  for(int i = 1; i < k; i++) {
    dp.push_back(vector<ll>(1, 0));
    for(int j = 1; j <= (int) sqrt(n); j++) {
      dp[i].push_back((dp[i][j-1] + n / j * 2 - 1 - (j - 1) * 2) % MOD);
    }
  }
  for(int j = 0; j < dp[k-1].size(); j++) cout << dp[k-1][j] << endl;
  return 0;
}
