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

  int n, c;
  cin >> n >> c;
  vector<vector<int> > d(c, vector<int>(c));
  for (auto &x: d) {
    for (auto &y: x) cin >> y;
  }
  vector<vector<int> > a(n, vector<int>(n));
  for (auto &x: a) {
    for (auto &y: x) {
      cin >> y;
      y--;
    }
  }

  vector<int> c0Count(c), c1Count(c), c2Count(c);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if ((i + j) % 3 == 0) c0Count[a[i][j]]++;
      else if ((i + j) % 3 == 1) c1Count[a[i][j]]++;
      else if ((i + j) % 3 == 2) c2Count[a[i][j]]++;
    }
  }

  int ans = 1000000007;
  for (int c0 = 0; c0 < c; c0++) {
    for (int c1 = 0; c1 < c; c1++) {
      if (c0 == c1) continue;
      for (int c2 = 0; c2 < c; c2++) {
        if (c0 == c2 || c1 == c2) continue;
        int cost = 0;
        for (int i = 0; i < c; i++) {
          cost += d[i][c0] * c0Count[i];
          cost += d[i][c1] * c1Count[i];
          cost += d[i][c2] * c2Count[i];
        }
        if (cost < ans) ans = cost;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
