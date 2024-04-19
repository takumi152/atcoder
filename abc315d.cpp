#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<vector<char>> c(h, vector<char>(w));
  for (auto &x: c) {
    for (auto &y: x) cin >> y;
  }

  vector<vector<int>> remaining_horizontal(h, vector<int>(26));
  vector<vector<int>> remaining_vertical(w, vector<int>(26));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      remaining_horizontal[i][c[i][j] - 'a']++;
      remaining_vertical[j][c[i][j] - 'a']++;
    }
  }

  unordered_set<int> exist_horizontal, exist_vertical;
  for (int i = 0; i < h; i++) exist_horizontal.insert(i);
  for (int i = 0; i < w; i++) exist_vertical.insert(i);

  while (true) {
    bool is_removed = false;

    vector<Pii> horizontal_to_be_removed;
    if ((int) exist_vertical.size() >= 2) {
      for (auto &i: exist_horizontal) {
        int existing_kind = 0;
        int existing_type = 0;
        for (int t = 0; t < 26; t++) {
          if (remaining_horizontal[i][t] > 0) {
            existing_kind++;
            existing_type = t;
          }
        }
        if (existing_kind == 1) {
          horizontal_to_be_removed.emplace_back(i, existing_type);
        }
      }
    }

    vector<Pii> vertical_to_be_removed;
    if ((int) exist_horizontal.size() >= 2) {
      for (auto &j: exist_vertical) {
        int existing_kind = 0;
        int existing_type = 0;
        for (int t = 0; t < 26; t++) {
          if (remaining_vertical[j][t] > 0) {
            existing_kind++;
            existing_type = t;
          }
        }
        if (existing_kind == 1) {
          vertical_to_be_removed.emplace_back(j, existing_type);
        }
      }
    }

    for (auto &[i, t]: horizontal_to_be_removed) {
      exist_horizontal.erase(i);
      for (auto &j: exist_vertical) {
        remaining_vertical[j][t]--;
      }
      is_removed = true;
    }

    for (auto &[j, t]: vertical_to_be_removed) {
      exist_vertical.erase(j);
      for (auto &i: exist_horizontal) {
        remaining_horizontal[i][t]--;
      }
      is_removed = true;
    }

    if (!is_removed) break;
  }

  int ans = 0;
  for (auto &i: exist_horizontal) {
    for (int t = 0; t < 26; t++) {
      ans += remaining_horizontal[i][t];
    }
  }

  cout << ans << endl;

  return 0;
}
