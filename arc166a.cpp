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

  int t;
  cin >> t;
  vector<int> n(t);
  vector<string> x(t), y(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i] >> x[i] >> y[i];
  }

  vector<string> ans(t);
  for (int i = 0; i < t; i++) {
    int ax = 0, ay = 0;
    int bx = 0, by = 0;
    int cx = 0;
    bool good = true;
    for (int j = 0; j < n[i]; j++) {
      if (x[i][j] == 'A') ax++;
      if (x[i][j] == 'B') bx++;
      if (x[i][j] == 'C') cx++;
      if (y[i][j] == 'A') ay++;
      if (y[i][j] == 'B') by++;
      if (y[i][j] == 'C') {
        if (x[i][j] != 'C') {
          good = false;
          break;
        }
        if (ax > ay) {
          good = false;
          break;
        }
        ax = 0;
        ay = 0;
        bx = 0;
        by = 0;
        cx = 0;
      }

      if (ax < ay) {
        if (cx > 0) {
          cx--;
          ax++;
        }
        else {
          good = false;
          break;
        }
      }
    }
    if (ax > ay) good = false;
    ans[i] = good ? "Yes" : "No";
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
