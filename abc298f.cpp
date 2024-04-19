#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

inline ll serialized_pos(int x, int y) {
  return (ll) x * 1000000000LL + (ll) y;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> r(n), c(n), x(n);
  for (int i = 0; i < n; i++) {
    cin >> r[i] >> c[i] >> x[i];
  }

  unordered_map<ll, int> num_exist_pos;
  for (int i = 0; i < n; i++) {
    num_exist_pos[serialized_pos(r[i], c[i])] = i;
  }

  unordered_map<int, ll> row_sum, col_sum;
  for (int i = 0; i < n; i++) {
    row_sum[r[i]] += x[i];
    col_sum[c[i]] += x[i];
  }

  vector<pair<ll, int>> row_sum_pos, col_sum_pos;
  for (auto &[row, sum]: row_sum) {
    row_sum_pos.emplace_back(sum, row);
  }
  for (auto &[col, sum]: col_sum) {
    col_sum_pos.emplace_back(sum, col);
  }
  sort(row_sum_pos.rbegin(), row_sum_pos.rend());
  sort(col_sum_pos.rbegin(), col_sum_pos.rend());

  ll ans = 0;
  priority_queue<tuple<ll, int, int>> que;
  que.emplace(row_sum_pos[0].first + col_sum_pos[0].first, 0, 0);
  unordered_set<ll> visited;
  while (!que.empty()) {
    auto [sum, i, j] = que.top();
    que.pop();
    if (visited.find(serialized_pos(i, j)) != visited.end()) continue;
    visited.insert(serialized_pos(i, j));
    if (sum < ans) break;
    int row = row_sum_pos[i].second;
    int col = col_sum_pos[j].second;
    if (num_exist_pos.find(serialized_pos(row, col)) == num_exist_pos.end()) {
      ans = sum;
      break;
    }
    else {
      int idx = num_exist_pos[serialized_pos(row, col)];
      ans = max(ans, sum - x[idx]);
      if (i < (int) row_sum_pos.size()-1 && visited.find(serialized_pos(i+1, j)) == visited.end()) {
        que.emplace(row_sum_pos[i+1].first + col_sum_pos[j].first, i+1, j);
      }
      if (j < (int) row_sum_pos.size()-1 && visited.find(serialized_pos(i, j+1)) == visited.end()) {
        que.emplace(row_sum_pos[i].first + col_sum_pos[j+1].first, i, j+1);
      }
    }
  }

  cout << ans << endl;

  return 0;
}
