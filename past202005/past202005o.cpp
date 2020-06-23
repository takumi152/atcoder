// WIP

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<ll> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;
  vector<ll> r(3);
  for (auto &x: r) cin >> x;

  vector<vector<ll> > score(n, vector<ll>(8));
  for (int i = 0; i < n; i++) {
    score[i][0] = 0;
    score[i][1] = (a[i] * b[i]) % r[0] - (a[i] * b[i]);
    score[i][2] = (a[i] * b[i] * b[i]) % r[1] - (a[i] * b[i]);
    score[i][3] = (a[i] * b[i]) % r[0] + (a[i] * b[i] * b[i]) % r[1] - (a[i] * b[i] * b[i]);
    score[i][4] = (a[i] * b[i] * b[i] * b[i]) % r[2] - (a[i] * b[i]);
    score[i][5] = (a[i] * b[i]) % r[0] + (a[i] * b[i] * b[i] * b[i]) % r[2] - (a[i] * b[i] * b[i]);
    score[i][6] = (a[i] * b[i] * b[i]) % r[1] + (a[i] * b[i] * b[i] * b[i]) % r[2] - (a[i] * b[i] * b[i]);
    score[i][7] = (a[i] * b[i]) % r[0] + (a[i] * b[i] * b[i]) % r[1] + (a[i] * b[i] * b[i] * b[i]) % r[2] - (a[i] * b[i] * b[i] * b[i]);
  }

  vector<vector<vector<ll> > > dp(m+2, vector<vector<ll> >(m+2, vector<ll>(m+2, (ll) -9e18)));
  dp[0][0][0] = 0;
  for (int i = 0; i < n; i++) {
    for (int j1 = min(m, i); j1 >= max(0, (m-n)+i); j1--) {
      for (int j2 = min(m, i); j2 >= max(0, (m-n)+i); j2--) {
        for (int j3 = min(m, i); j3 >= max(0, (m-n)+i); j3--) {
          dp[j1+1][j2][j3] = max(dp[j1+1][j2][j3], dp[j1][j2][j3] + score[i][1]);
          dp[j1][j2+1][j3] = max(dp[j1][j2+1][j3], dp[j1][j2][j3] + score[i][2]);
          dp[j1+1][j2+1][j3] = max(dp[j1+1][j2+1][j3], dp[j1][j2][j3] + score[i][3]);
          dp[j1][j2][j3+1] = max(dp[j1][j2][j3+1], dp[j1][j2][j3] + score[i][4]);
          dp[j1+1][j2][j3+1] = max(dp[j1+1][j2][j3+1], dp[j1][j2][j3] + score[i][5]);
          dp[j1][j2+1][j3+1] = max(dp[j1][j2+1][j3+1], dp[j1][j2][j3] + score[i][6]);
          dp[j1+1][j2+1][j3+1] = max(dp[j1+1][j2+1][j3+1], dp[j1][j2][j3] + score[i][7]);
        }
      }
    }
  }

  cout << dp[m][m][m] << endl;

  return 0;
}
