#include <iostream>
#include <iomanip>
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

  int h, w, n, m;
  cin >> h >> w >> n >> m;
  vector<vector<bool> > light(h, vector<bool>(w));
  for (int i = 0; i < n; i++) {
    int a, b;
    cin >> a >> b;
    light[a-1][b-1] = true;
  }
  vector<vector<bool> > block(h, vector<bool>(w));
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    block[a-1][b-1] = true;
  }

  vector<vector<vector<bool> > > reachable(h, vector<vector<bool> >(w, vector<bool>(4)));
  vector<int> dx = {1, 0, -1, 0};
  vector<int> dy = {0, 1, 0, -1};
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (light[i][j]) {
        for (int k = 0; k < 4; k++) {
          int x = i;
          int y = j;
          while (x >= 0 && x < h && y >= 0 && y < w) {
            if (block[x][y] || reachable[x][y][k]) break;
            reachable[x][y][k] = true;
            x += dx[k];
            y += dy[k];
          }
        }
      }
    }
  }

  int ans = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (reachable[i][j][0] || reachable[i][j][1] || reachable[i][j][2] || reachable[i][j][3]) ans++;
    }
  }

  cout << ans << endl;

  return 0;
}
