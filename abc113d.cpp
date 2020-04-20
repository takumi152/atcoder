#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w, k;
  cin >> h >> w >> k;

  vector<vector<ll> > dp(h+1, vector<ll>(w));
  dp[0][0] = 1;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < (1 << (w-1)); j++) {
      int m = j;
      int p = 0;
      vector<int> next(w);
      for (int l = 0; l < w; l++) next[l] = l;
      bool good = true;
      while (m > 0) {
        if ((m & 3) == 3) {
          good = false;
          break;
        }
        else if (m & 1) {
          swap(next[p], next[p+1]);
        }
        p++;
        m >>= 1;
      }
      if (!good) continue;
      for (int l = 0; l < w; l++) {
        dp[i+1][next[l]] = (dp[i+1][next[l]] + dp[i][l]) % mod;
      }
    }
  }

  cout << dp[h][k-1] << endl;

  return 0;
}
