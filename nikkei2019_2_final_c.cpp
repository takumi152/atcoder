#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w, k;
  cin >> h >> w >> k;
  vector<Pii> ab(k);
  for (auto &x: ab) cin >> x.first >> x.second;

  vector<vector<bool> > board(h, vector<bool>(w, true));
  for (auto &x: ab) board[x.first-1][x.second-1] = false;

  vector<vector<int> > hor(h, vector<int>(w+1)), ver(h+1, vector<int>(w)), diag(h+1, vector<int>(w+1));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      hor[i][j+1] = hor[i][j];
      if (board[i][j]) hor[i][j+1]++;
    }
  }

  for (int j = 0; j < w; j++) {
    for (int i = 0; i < h; i++) {
      ver[i+1][j] = ver[i][j];
      if (board[i][j]) ver[i+1][j]++;
    }
  }

  for (int k = 0; k < h + w; k++) {
    int x = max(0, h - k);
    int y = max(0, k - h);
    while (x < h && y < w) {
      diag[x+1][y+1] = diag[x][y];
      if (board[x][y]) diag[x+1][y+1]++;
      x++;
      y++;
    }
  }

  int best = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      for (int k = best; i + k < h && j + k < w; k++) {
        if (ver[i+k+1][j] - ver[i][j] == k+1 && ver[i+k+1][j+k] - ver[i][j+k] == k+1 && diag[i+k+1][j+k+1] - diag[i][j] == k+1) best = k+1;
        else if (ver[i+k+1][j] - ver[i][j] < k+1 || diag[i+k+1][j+k+1] - diag[i][j] < k+1) break;
      }
    }
  }

  cout << best << endl;

  return 0;
}
