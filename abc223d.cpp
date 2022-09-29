#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
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
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
  }

  for (int i = 0; i < n; i++) sort(edge[i].begin(), edge[i].end());

  vector<int> reference_count(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: edge[i]) reference_count[x]++;
  }

  vector<int> ans;
  priority_queue<int, vector<int>, greater<int> > que;
  for (int i = 0; i < n; i++) {
    if (reference_count[i] == 0) que.push(i);
  }
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    ans.push_back(now+1);
    for (auto &x: edge[now]) {
      reference_count[x]--;
      if (reference_count[x] == 0) que.push(x);
    }
  }

  if ((int) ans.size() == n) {
    for (auto &x: ans) cout << x << " ";
    cout << endl;
  }
  else {
    cout << -1 << endl;
  }

  return 0;
}
