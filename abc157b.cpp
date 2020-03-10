#include <iostream>
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

  vector<vector<int> > a(3, vector<int>(3));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }
  int n;
  cin >> n;
  vector<int> b(n);
  for (auto &x: b) cin >> x;

  vector<vector<bool> > open(3, vector<bool>(3));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (a[i][j] == b[k]) {
          open[i][j] = true;
        }
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    if (open[i][0] && open[i][1] && open[i][2]) {
      cout << "Yes" << endl;
      return 0;
    }
  }
  for (int j = 0; j < 3; j++) {
    if (open[0][j] && open[1][j] && open[2][j]) {
      cout << "Yes" << endl;
      return 0;
    }
  }
  if (open[0][0] && open[1][1] && open[2][2]) {
    cout << "Yes" << endl;
    return 0;
  }
  if (open[0][2] && open[1][1] && open[2][0]) {
    cout << "Yes" << endl;
    return 0;
  }

  cout << "No" << endl;

  return 0;
}
