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

  int n, m;
  cin >> n >> m;
  vector<vector<int>> s(n);
  for (auto &x: s) {
    int a;
    cin >> a;
    x = vector<int>(a);
    for (auto &y: x) cin >> y;
    sort(x.begin(), x.end());
  }

  vector<vector<int>> existing_set_of_number(m+1);
  for (int i = 0; i < n; i++) {
    for (auto &x: s[i]) {
      existing_set_of_number[x].push_back(i);
    }
  }

  auto ll_hash = [](int &x, int &y) {return (ll) x * (ll) 1e9 + (ll) y;};

  unordered_set<ll> visited;
  deque<tuple<int, int, int, bool>> que; // set_id, num, cost, should_search_same_set
  for (int i = 0; i < n; i++) {
    if (s[i][0] == 1) {
      que.emplace_back(i, 1, 0, true);
    }
  }

  int ans = -1;
  while (!que.empty()) {
    auto [set_id, num, cost, should_search_same_set] = que.front();
    que.pop_front();
    ll hash_code_now = ll_hash(set_id, num);
    if (visited.find(hash_code_now) != visited.end()) continue;
    visited.insert(hash_code_now);
    if (num == m) {
      ans = cost;
      break;
    }

    if (should_search_same_set) {
      for (auto &x: s[set_id]) {
        ll hash_code_next = ll_hash(set_id, x);
        if (visited.find(hash_code_next) == visited.end()) que.emplace_front(set_id, x, cost, false);
      }
    }

    for (auto &x: existing_set_of_number[num]) {
      ll hash_code_next = ll_hash(x, num);
      if (visited.find(hash_code_next) == visited.end()) que.emplace_back(x, num, cost + 1, true);
    }
  }

  cout << ans << endl;

  return 0;
}
