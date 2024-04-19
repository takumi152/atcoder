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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<vector<char>> a(h, vector<char>(w));
  vector<vector<char>> b(h, vector<char>(w));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }
  for (auto &x: b) {
    for (auto &y: x) cin >> y;
  }

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      bool good = true;
      for (int x = 0; x < h; x++) {
        for (int y = 0; y < w; y++) {
          if (a[x][y] != b[(i + x) % h][(j + y) % w]) {
            good = false;
            break;
          }
        }
      }
      if (good) {
        cout << "Yes" << endl;
        return 0;
      }
    }
  }

  cout << "No" << endl;

  return 0;
}
