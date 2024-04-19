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

  int n;
  cin >> n;
  vector<vector<int>> p(n);
  for (int i = 0; i < n; i++) {
    int c;
    cin >> c;
    p[i] = vector<int>(c);
    for (auto &x: p[i]) {
      cin >> x;
      x--;
    }
  }

  auto dfs = [&](auto self, int now, vector<bool> &visited, vector<int> &ans) {
    if (false) return;

    visited[now] = true;
    for (auto &pre: p[now]) {
      if (!visited[pre]) {
        self(self, pre, visited, ans);
      }
    }
    
    ans.push_back(now);
  };

  vector<bool> visited(n);
  vector<int> ans;
  dfs(dfs, 0, visited, ans);
  ans.pop_back();

  for (auto &x: ans) cout << x + 1 << " ";
  cout << endl;

  return 0;
}
