#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<vector<int>> a(h, vector<int>(w));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }
  vector<vector<int>> b(h, vector<int>(w));
  for (auto &x: b) {
    for (auto &y: x) cin >> y;
  }

  vector<int> row_order(h);
  for (int i = 0; i < h; i++) row_order[i] = i;
  vector<int> col_order(w);
  for (int j = 0; j < w; j++) col_order[j] = j;
  int ans = 1000000000;
  do {
    do {
      bool match = true;
      for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
          if (a[row_order[i]][col_order[j]] != b[i][j]) match = false;
        }
      }
      if (match) {
        int score = 0;
        for (int i1 = 0; i1 < h; i1++) {
          for (int i2 = i1 + 1; i2 < h; i2++) {
            if (row_order[i1] > row_order[i2]) score++;
          }
        }
        for (int j1 = 0; j1 < w; j1++) {
          for (int j2 = j1 + 1; j2 < w; j2++) {
            if (col_order[j1] > col_order[j2]) score++;
          }
        }
        ans = min(ans, score);
      }
    } while (next_permutation(col_order.begin(), col_order.end()));
  } while (next_permutation(row_order.begin(), row_order.end()));

  if (ans == 1000000000) cout << -1 << endl;
  else cout << ans << endl;

  return 0;
}
