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
  vector<vector<pair<int, int> > > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].emplace_back(b-1, i);
    edge[b-1].emplace_back(a-1, i);
  }

  vector<int> ans(n-1);
  int maxK = 0;
  stack<pair<int, int> > st;
  st.emplace(0, 0);
  while (!st.empty()) {
    auto now = st.top();
    st.pop();
    int v = now.first;
    int last = now.second;
    int col = 1;
    for (auto &x: edge[v]) {
      if (col == last) col++;
      if (ans[x.second] > 0) continue;
      ans[x.second] = col;
      st.emplace(x.first, col);
      maxK = max(maxK, col);
      col++;
    }
  }

  cout << maxK << endl;
  for (auto &x: ans) cout << x << endl;

  return 0;
}
