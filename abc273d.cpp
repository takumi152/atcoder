#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w, rs, cs;
  cin >> h >> w >> rs >> cs;
  int n;
  cin >> n;
  unordered_map<int, vector<int>> wall_row, wall_col;
  for (int i = 0; i < n; i++) {
    int wr, wc;
    cin >> wr >> wc;
    wall_row[wr].push_back(wc);
    wall_col[wc].push_back(wr);
  }
  int q;
  cin >> q;
  vector<pair<char, int>> query(q);
  for (auto &x: query) cin >> x.first >> x.second;

  for (auto &[_, row]: wall_row) sort(row.begin(), row.end());
  for (auto &[_, col]: wall_col) sort(col.begin(), col.end());
  
  vector<Pii> ans;
  int px = rs;
  int py = cs;
  for (auto &[dir, len]: query) {
    if (dir == 'L') {
      int py_next = py - len;
      int left_min = 1;
      if (!wall_row[px].empty()) {
        auto it = lower_bound(wall_row[px].begin(), wall_row[px].end(), py);
        if (it != wall_row[px].begin()) left_min = *(it - 1) + 1;
      }
      py = max(left_min, py_next);
    }
    else if (dir == 'R') {
      int py_next = py + len;
      int right_max = w;
      if (!wall_row[px].empty()) {
        auto it = lower_bound(wall_row[px].begin(), wall_row[px].end(), py);
        if (it != wall_row[px].end()) right_max = *(it) - 1;
      }
      py = min(py_next, right_max);
    }
    else if (dir == 'U') {
      int px_next = px - len;
      int up_min = 1;
      if (!wall_col[py].empty()) {
        auto it = lower_bound(wall_col[py].begin(), wall_col[py].end(), px);
        if (it != wall_col[py].begin()) up_min = *(it - 1) + 1;
      }
      px = max(up_min, px_next);
    }
    else if (dir == 'D') {
      int px_next = px + len;
      int down_max = h;
      if (!wall_col[py].empty()) {
        auto it = lower_bound(wall_col[py].begin(), wall_col[py].end(), px);
        if (it != wall_col[py].end()) down_max = *(it) - 1;
      }
      px = min(px_next, down_max);
    }

    ans.emplace_back(px, py);
  }

  for (auto &[x, y]: ans) cout << x << " " << y << endl;

  return 0;
}
