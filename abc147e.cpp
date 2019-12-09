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

  int h, w;
  cin >> h >> w;
  vector<vector<int> > a(h, vector<int>(w));
  vector<vector<int> > b(h, vector<int>(w));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }
  for (auto &x: b) {
    for (auto &y: x) cin >> y;
  }

  vector<vector<int> > d(h, vector<int>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) d[i][j] = abs(a[i][j] - b[i][j]);
  }

  vector<vector<vector<bool> > > dp(h, vector<vector<bool> >(w, vector<bool>(30000)));
  dp[0][0][15000+d[0][0]] = true;
  dp[0][0][15000-d[0][0]] = true;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (i < h-1) {
        for (int k = 100; k < 29900; k++) {
          if (dp[i][j][k]) {
            dp[i+1][j][k+d[i+1][j]] = true;
            dp[i+1][j][k-d[i+1][j]] = true;
          }
        }
      }
      if (j < w-1) {
        for (int k = 100; k < 29900; k++) {
          if (dp[i][j][k]) {
            dp[i][j+1][k+d[i][j+1]] = true;
            dp[i][j+1][k-d[i][j+1]] = true;
          }
        }
      }
    }
  }

  int best = 1000000007;
  for (int k = 0; k < 30000; k++) {
    if (dp[h-1][w-1][k]) {
      if (abs(k - 15000) < best) best = abs(k - 15000);
    }
  }

  cout << best << endl;

  return 0;
}
