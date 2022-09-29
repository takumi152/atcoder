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
    edge[b-1].push_back(a-1);
  }

  int one_or_less = 0;
  for (auto &x: edge) {
    if ((int) x.size() > 2) {
      cout << "No" << endl;
      return 0;
    }
    if ((int) x.size() <= 1) one_or_less++;
  }

  if (one_or_less < 2) {
    cout << "No" << endl;
    return 0;
  }

  vector<bool> visited(n);
  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      queue<Pii> que;
      que.emplace(-1, i);
      while (!que.empty()) {
        auto [prev, now] = que.front();
        que.pop();
        if (visited[now]) {
          cout << "No" << endl;
          return 0;
        }
        visited[now] = true;
        for (auto &next: edge[now]) {
          if (next == prev) continue;
          else que.emplace(now, next);
        }
      }
    }
  }

  cout << "Yes" << endl;

  return 0;
}
