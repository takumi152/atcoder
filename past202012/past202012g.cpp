#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

bool solve(int x, int y, int depth, int h, int w, int k, vector<string> &s, vector<vector<bool> > &visited, vector<Pii> &ans) {
  visited[x][y] = true;
  ans.emplace_back(x+1, y+1);

  if (depth == k) return true;

  const vector<Pii> delta = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (auto &[dx, dy]: delta) {
    if (x+dx < 0 || x+dx >= h || y+dy < 0 || y+dy >= w) continue;
    if (visited[x+dx][y+dy] || s[x+dx][y+dy] != '#') continue;
    if (solve(x+dx, y+dy, depth+1, h, w, k, s, visited, ans)) return true;
  }

  ans.pop_back();
  visited[x][y] = false;

  return false;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  int k = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '#') k++;
    }
  }

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] != '#') continue;
      vector<vector<bool> > visited(h, vector<bool>(w));
      vector<Pii> ans;
      if (solve(i, j, 1, h, w, k, s, visited, ans)) {
        cout << k << endl;
        for (auto &[x, y]: ans) cout << x << " " << y << endl;
        return 0;
      }
    }
  }

  return 0;
}
