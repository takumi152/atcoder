#include <iostream>
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

void rotate_90_deg_ccw(vector<vector<char> > &a) {
  int n = a.size();

  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) swap(a[i][j], a[j][i]);
  }

  reverse(a.begin(), a.end());
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> board(h);
  for (auto &x: board) cin >> x;

  vector<vector<vector<char> > > a(4, vector<vector<char> >(7, vector<char>(7, '.')));
  a[0][1][3] = 'o';
  a[0][2][2] = 'o';
  a[0][2][4] = 'o';
  a[0][3][1] = 'o';
  a[0][3][5] = 'o';
  a[0][4][1] = 'o';
  a[0][4][2] = 'o';
  a[0][4][3] = 'o';
  a[0][4][4] = 'o';
  a[0][4][5] = 'o';
  a[0][5][1] = 'o';
  a[0][5][5] = 'o';
  for (int i = 1; i < 4; i++) {
    a[i] = a[i-1];
    rotate_90_deg_ccw(a[i]);
  }

  vector<vector<vector<char> > > b(4, vector<vector<char> >(7, vector<char>(7, '.')));
  b[0][1][1] = 'o';
  b[0][1][2] = 'o';
  b[0][1][3] = 'o';
  b[0][1][4] = 'o';
  b[0][2][1] = 'o';
  b[0][2][5] = 'o';
  b[0][3][1] = 'o';
  b[0][3][2] = 'o';
  b[0][3][3] = 'o';
  b[0][3][4] = 'o';
  b[0][4][1] = 'o';
  b[0][4][5] = 'o';
  b[0][5][1] = 'o';
  b[0][5][2] = 'o';
  b[0][5][3] = 'o';
  b[0][5][4] = 'o';
  for (int i = 1; i < 4; i++) {
    b[i] = b[i-1];
    rotate_90_deg_ccw(b[i]);
  }

  vector<vector<vector<char> > > c(4, vector<vector<char> >(7, vector<char>(7, '.')));
  c[0][1][2] = 'o';
  c[0][1][3] = 'o';
  c[0][1][4] = 'o';
  c[0][2][1] = 'o';
  c[0][2][5] = 'o';
  c[0][3][1] = 'o';
  c[0][4][1] = 'o';
  c[0][4][5] = 'o';
  c[0][5][2] = 'o';
  c[0][5][3] = 'o';
  c[0][5][4] = 'o';
  for (int i = 1; i < 4; i++) {
    c[i] = c[i-1];
    rotate_90_deg_ccw(c[i]);
  }

  vector<int> a_offset = {2, 2, 0, 0};
  vector<int> b_offset = {0, 1, 1, 0};
  vector<int> c_offset = {1, 1, 1, 1};

  int a_count = 0, b_count = 0, c_count = 0;
  vector<vector<bool> > visited(h, vector<bool>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (visited[i][j]) continue;
      if (board[i][j] == '.') continue;

      int scale = 1;
      while (true) {
        bool good = true;
        for (int k = 0; k <= scale; k++) {
          if (i+k >= h || j+scale >= w) {
            good = false;
            break;
          }
          if (board[i+k][j+scale] == '.') {
            good = false;
            break;
          }
        }
        for (int l = 0; l <= scale; l++) {
          if (i+scale >= h || j+l >= w) {
            good = false;
            break;
          }
          if (board[i+scale][j+l] == '.') {
            good = false;
            break;
          }
        }
        if (good) scale++;
        else break;
      }

      bool found = false;

      for (int p = 0; p < 4; p++) {
        bool good = true;
        for (int k = 0; k < 5; k++) {
          for (int l = 0; l < 5; l++) {
            if (i+k*scale >= h || j+(l-a_offset[p])*scale < 0 || j+(l-a_offset[p])*scale >= w) {
              good = false;
              break;
            }
            if (board[i+k*scale][j+(l-a_offset[p])*scale] != a[p][k+1][l+1]) {
              good = false;
              break;
            }
          }
        }
        if (good) {
          a_count++;
          for (int k = 0; k < 7; k++) {
            for (int l = 0; l < 7; l++) {
              for (int sx = 0; sx < scale; sx++) {
                for (int sy = 0; sy < scale; sy++) {
                  visited[i+(k-1)*scale+sx][j+((l-1)-a_offset[p])*scale+sy] = true;
                }
              }
            }
          }
        }
      }
      if (found) break;

      for (int p = 0; p < 4; p++) {
        bool good = true;
        for (int k = 0; k < 5; k++) {
          for (int l = 0; l < 5; l++) {
            if (i+k*scale >= h || j+(l-b_offset[p])*scale < 0 || j+(l-b_offset[p])*scale >= w) {
              good = false;
              break;
            }
            if (board[i+k*scale][j+(l-b_offset[p])*scale] != b[p][k+1][l+1]) {
              good = false;
              break;
            }
          }
        }
        if (good) {
          b_count++;
          for (int k = 0; k < 7; k++) {
            for (int l = 0; l < 7; l++) {
              for (int sx = 0; sx < scale; sx++) {
                for (int sy = 0; sy < scale; sy++) {
                  visited[i+(k-1)*scale+sx][j+((l-1)-b_offset[p])*scale+sy] = true;
                }
              }
            }
          }
        }
      }
      if (found) break;

      for (int p = 0; p < 4; p++) {
        bool good = true;
        for (int k = 0; k < 5; k++) {
          for (int l = 0; l < 5; l++) {
            if (i+k*scale >= h || j+(l-c_offset[p])*scale < 0 || j+(l-c_offset[p])*scale >= w) {
              good = false;
              break;
            }
            if (board[i+k*scale][j+(l-c_offset[p])*scale] != c[p][k+1][l+1]) {
              good = false;
              break;
            }
          }
        }
        if (good) {
          c_count++;
          for (int k = 0; k < 7; k++) {
            for (int l = 0; l < 7; l++) {
              for (int sx = 0; sx < scale; sx++) {
                for (int sy = 0; sy < scale; sy++) {
                  visited[i+(k-1)*scale+sx][j+((l-1)-c_offset[p])*scale+sy] = true;
                }
              }
            }
          }
        }
      }
      if (found) break;
    }
  }

  cout << a_count << " " << b_count << " " << c_count << endl;

  return 0;
}
