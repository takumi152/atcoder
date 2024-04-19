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
  vector<vector<int>> a(n, vector<int>(n));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }
  vector<vector<int>> b(n, vector<int>(n));
  for (auto &x: b) {
    for (auto &y: x) cin >> y;
  }

  bool good_any = false;
  {
    bool good = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (!(a[i][j] == 0 || (a[i][j] == 1 && b[i][j] == 1))) {
          good = false;
        }
      }
    }
    if (good) good_any = true;
  }
  {
    bool good = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (!(a[n-j-1][i] == 0 || (a[n-j-1][i] == 1 && b[i][j] == 1))) {
          good = false;
        }
      }
    }
    if (good) good_any = true;
  }
  {
    bool good = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (!(a[n-i-1][n-j-1] == 0 || (a[n-i-1][n-j-1] == 1 && b[i][j] == 1))) {
          good = false;
        }
      }
    }
    if (good) good_any = true;
  }
  {
    bool good = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (!(a[j][n-i-1] == 0 || (a[j][n-i-1] == 1 && b[i][j] == 1))) {
          good = false;
        }
      }
    }
    if (good) good_any = true;
  }

  if (good_any) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
