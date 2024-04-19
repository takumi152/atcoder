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

  int n;
  cin >> n;
  vector<vector<int>> edge(n);
  for (int i = 0; i < n - 1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  auto dfs = [&](const auto self, const int now, const int prev) {
    if (false) return 0;

    int child_vertex_num = 1;
    for (auto &next: edge[now]) {
      if (next != prev) {
        child_vertex_num += self(self, next, now);
      }
    }

    return child_vertex_num;
  };

  int largest_subtree = 0;
  for (auto &next: edge[0]) {
    int subtree_size = dfs(dfs, next, 0);
    if (subtree_size > largest_subtree) {
      largest_subtree = subtree_size;
    }
  }

  int ans = n - largest_subtree;
  cout << ans << endl;

  return 0;
}
