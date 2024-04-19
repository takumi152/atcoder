#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct unionfind {
  vector<int> group;
  vector<int> rank;

  unionfind() {
    unionfind(0);
  }

  unionfind(int n) {
    group = vector<int>(n);
    rank = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rank[rx] > rank[ry]) group[ry] = rx;
    else if (rank[ry] > rank[rx]) group[rx] = ry;
    else if (rx != ry) {
      group[ry] = rx;
      rank[rx]++;
    }
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  int m = n * (n - 1) / 2;
  vector<Pii> ab(m);
  for (auto &[x, y]: ab) {
    cin >> x >> y;
    x--;
    y--;
  }

  if (n == 2) {
    cout << 2 << endl;
    return 0;
  }

  vector<vector<int>> edge_id(n, vector<int>(n));
  {
    int id = 0;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        edge_id[i][j] = id;
        id++;
      }
    }
  }

  vector<vector<int>> edge_priority(n, vector<int>(n));
  for (int i = 0; i < m; i++) {
    auto [a, b] = ab[i];
    edge_priority[a][b] = i;
  }

  unionfind uf(m);

  queue<tuple<int, int, int>> triangles;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      for (int k = j + 1; k < n; k++) {
        triangles.emplace(i, j, k);
      }
    }
  }

  vector<vector<bool>> is_determined(n, vector<bool>(n));
  vector<vector<bool>> smaller_is_smaller(n, vector<bool>(n));

  auto check_and_set_comparison = [&](int base, int other1, int other2, bool is_first, bool &is_set, bool &is_contradicted) {
    int high1 = (base > other1) ? base : other1;
    int low1 = (high1 == base) ? other1 : base;
    int high2 = (base > other2) ? base : other2;
    int low2 = (high2 == base) ? other2 : base;
    bool should_invert = (base > other1) != (base > other2);
    if (!is_determined[low1][high1] && !is_determined[low2][high2]) {
      if (is_first) {
        is_determined[low1][high1] = true;
        is_determined[low2][high2] = true;
        smaller_is_smaller[low1][high1] = false;
        smaller_is_smaller[low2][high2] = should_invert;
        uf.unite(edge_id[low1][high1], edge_id[low2][high2]);
      }
      is_set = false;
      is_contradicted = false;
    }
    else if (!is_determined[low1][high1]) {
      is_determined[low1][high1] = true;
      smaller_is_smaller[low1][high1] = smaller_is_smaller[low2][high2] != should_invert;
      is_set = true;
      is_contradicted = false;
      uf.unite(edge_id[low1][high1], edge_id[low2][high2]);
    }
    else if (!is_determined[low2][high2]) {
      is_determined[low2][high2] = true;
      smaller_is_smaller[low2][high2] = smaller_is_smaller[low1][high1] != should_invert;
      is_set = true;
      is_contradicted = false;
      uf.unite(edge_id[low1][high1], edge_id[low2][high2]);
    }
    else {
      is_set = true;
      is_contradicted = (smaller_is_smaller[low1][high1] != smaller_is_smaller[low2][high2]) != should_invert;
      uf.unite(edge_id[low1][high1], edge_id[low2][high2]);
    }
  };

  {
    auto [a, b, c] = triangles.front();
    triangles.pop();
    bool is_set = false;
    bool is_contradicted = false;
    if      (edge_priority[a][b] < edge_priority[b][c] && edge_priority[a][c] < edge_priority[b][c]) check_and_set_comparison(a, b, c, true, is_set, is_contradicted);
    else if (edge_priority[a][b] < edge_priority[a][c] && edge_priority[b][c] < edge_priority[a][c]) check_and_set_comparison(b, a, c, true, is_set, is_contradicted);
    else                                                                                             check_and_set_comparison(c, a, b, true, is_set, is_contradicted);
  }

  while (!triangles.empty()) {
    auto [a, b, c] = triangles.front();
    triangles.pop();
    bool is_set = false;
    bool is_contradicted = false;
    if      (edge_priority[a][b] < edge_priority[b][c] && edge_priority[a][c] < edge_priority[b][c]) check_and_set_comparison(a, b, c, false, is_set, is_contradicted);
    else if (edge_priority[a][b] < edge_priority[a][c] && edge_priority[b][c] < edge_priority[a][c]) check_and_set_comparison(b, a, c, false, is_set, is_contradicted);
    else                                                                                             check_and_set_comparison(c, a, b, false, is_set, is_contradicted);
    if (!is_set) triangles.emplace(a, b, c);
    if (is_contradicted) {
      cout << 0 << endl;
      return 0;
    }
  }

  unordered_set<int> roots;
  for (int i = 0; i < m; i++) {
    roots.insert(uf.root(i));
  }

  int ans = 1;
  for (int i = 0; i < (int) roots.size(); i++) ans = (ans * 2) % mod;

  cout << ans << endl;

  return 0;
}
