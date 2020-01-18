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

  int n;
  cin >> n;
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<vector<vector<short> > > dp(n+1, vector<vector<short> > (1 << n, vector<short>(50+1, 10000)));
  dp[0][0][0] = 0;
  vector<vector<int> > mask(n+1);
  for (int i = 0; i < (1 << n); i++) mask[__builtin_popcount(i)].push_back(i);

  for (int k1 = 0; k1 < n; k1++) {
    for (int k2 = 0; k2 < n; k2++) {
      int t;
      if ((k1 & 1) == (k2 & 1)) t = a[k2];
      else t = b[k2];
      int m = (1 << k2);
      for (auto &i: mask[k1]) {
        if (i & m) continue;
        for (int j = 0; j <= t; j++) {
          dp[k1+1][i|m][t] = min(dp[k1+1][i|m][t], (short)(dp[k1][i][j] + __builtin_popcount(m-1) - __builtin_popcount((int)i & (m-1))));
        }
      }
    }
  }

  int best = 10000;
  for (int i = 0; i <= 50; i++) {
    if (dp[n][(1<<n)-1][i] < best) best = dp[n][(1<<n)-1][i];
  }

  if (best == 10000) best = -1;
  cout << best << endl;

/*
  for (auto &x: dp) {
    for (auto &y: x) {
      for (auto &z: y) {
        cerr << z << " ";
      }
      cerr << endl;
    }
    cerr << endl;
  }
*/

  return 0;
}
