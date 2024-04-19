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
  while (t--) {
    int n;
    cin >> n;
    vector<vector<int>> edge(n);
    for (int i = 0; i < n-1; i++) {
      int a, b;
      cin >> a >> b;
      edge[a-1].push_back(b-1);
      edge[b-1].push_back(a-1);
    }
    vector<char> s(n);
    for (auto &x: s) cin >> x;

    vector<bool> is_leaf(n);
    for (int i = 0; i < n; i++) {
      if ((int) edge[i].size() == 1) is_leaf[i] = true;
    }

    vector<int> non_leaf_adj_count(n);
    for (int i = 0; i < n; i++) {
      for (auto &x: edge[i]) {
        if (!is_leaf[x]) non_leaf_adj_count[i]++;
      }
    }

    vector<int> resolved_non_leaf_count(n);
    vector<int> w_count(n);
    vector<int> b_count(n);

    auto add_color_count = [&](char c, int idx) {
      if (c == 'W') w_count[idx]++;
      else if (c == 'B') b_count[idx]++;
    };

    bool good = true;
    vector<char> ans(n, '.');
    queue<int> que;
    for (int i = 0; i < n; i++) {
      if (!is_leaf[i] && (int) edge[i].size() - non_leaf_adj_count[i] > 0) {
        for (auto &x: edge[i]) {
          if (is_leaf[x]) {
            ans[x] = s[i];
            add_color_count(ans[x], i);
            if (ans[i] == '.') {
              ans[i] = s[x];
              for (auto &y: edge[i]) add_color_count(ans[i], y);
            }
            else if (ans[i] != s[x]) good = false;
          }
          resolved_non_leaf_count[x]++;
          if (resolved_non_leaf_count[x] == non_leaf_adj_count[x] - 1) que.push(x);
        }
      }
    }

    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (ans[now] != '.') continue;
      for (auto &x: edge[now]) {
        if (ans[x] != '.') {
          if (w_count[x] == b_count[x]) {
            if (ans[now] == '.') {
              ans[now] = s[x];
              for (auto &y: edge[now]) add_color_count(ans[now], y);
            }
          }
        }
      }
      if (ans[now] == '.') {
        for (auto &x: edge[now]) {
          if (ans[x] == '.') {
            ans[now] = s[x];
            for (auto &y: edge[now]) add_color_count(ans[now], y);
          }
        }
      }
      if (ans[now] == '.') {
        ans[now] = 'B';
        for (auto &y: edge[now]) add_color_count(ans[now], y);
      }
      for (auto &x: edge[now]) {
        resolved_non_leaf_count[x]++;
        if (resolved_non_leaf_count[x] == non_leaf_adj_count[x] - 1) que.push(x);
      }
    }

    for (int i = 0; i < n; i++) {
      if ((s[i] == 'W' && !(b_count[i] < w_count[i])) || (s[i] == 'B' && !(b_count[i] > w_count[i]))) good = false;
    }

    if (good) {
      for (auto &x: ans) cout << x;
      cout << endl;
    }
    else {
      cout << -1 << endl;
    }
  }

  return 0;
}
