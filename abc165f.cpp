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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;
  vector<vector<int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int u, v;
    cin >> u >> v;
    edge[u-1].push_back(v-1);
    edge[v-1].push_back(u-1);
  }

  vector<int> ans(n);
  stack<Pii> history;
  history.emplace(0, 0);
  vector<int> dp(n+1, 1000000007);
  dp[0] = 0;
  stack<Pii> st;
  st.emplace(0, 0);
  while (!st.empty()) {
    auto now = st.top();
    st.pop();
    if (now.second == 0) {
      auto it = lower_bound(dp.begin(), dp.end(), a[now.first]);
      int idx = distance(dp.begin(), it);
      history.emplace(idx, dp[idx]);
      dp[idx] = a[now.first];
    }
    if (now.second == (int) edge[now.first].size()) {
      auto it = lower_bound(dp.begin(), dp.end(), 1000000007);
      int idx = distance(dp.begin(), it) - 1;
      ans[now.first] = idx;
      // revert
      auto prev = history.top();
      history.pop();
      dp[prev.first] = prev.second;
    }
    else {
      if (st.empty()) {
        st.emplace(now.first, now.second+1);
        st.emplace(edge[now.first][now.second], 0);
      }
      else {
        int prev = st.top().first;
        st.emplace(now.first, now.second+1);
        if (prev != edge[now.first][now.second]) st.emplace(edge[now.first][now.second], 0);
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
