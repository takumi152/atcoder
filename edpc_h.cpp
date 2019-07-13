#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int H, W;
  cin >> H >> W;
  vector<string> a(H);
  for (auto &s: a) cin >> s;

  vector<vector<ll> > dp(H+2, vector<ll>(W+2));
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      if (i == 0 && j == 0) dp[i+1][j+1] = 1;
      else if (a[i][j] == '.') dp[i+1][j+1] = (dp[i+1][j] + dp[i][j+1]) % MOD;
    }
  }

  cout << dp[H][W] << endl;
  return 0;
}
