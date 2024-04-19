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

  int n, x, y;
  cin >> n >> x >> y;
  vector<int> a(n);
  for (auto &z: a) cin >> z;

  vector<int> horizontal;
  vector<int> vertical;
  for (int i = 1; i < n; i++) {
    if (i % 2 == 0) horizontal.push_back(a[i]);
    else vertical.push_back(a[i]);
  }

  vector<vector<bool>> dpx(horizontal.size()+1, vector<bool>(20001));
  dpx[0][10000+a[0]] = true;
  for (int i = 0; i < (int) horizontal.size(); i++) {
    for (int j = 0; j <= 20000; j++) {
      if (dpx[i][j]) {
        dpx[i+1][j+horizontal[i]] = true;
        dpx[i+1][j-horizontal[i]] = true;
      }
    }
  }
  if (!dpx[horizontal.size()][10000+x]) {
    cout << "No" << endl;
    return 0;
  }

  vector<vector<bool>> dpy(vertical.size()+1, vector<bool>(20001));
  dpy[0][10000] = true;
  for (int i = 0; i < (int) vertical.size(); i++) {
    for (int j = 0; j <= 20000; j++) {
      if (dpy[i][j]) {
        dpy[i+1][j+vertical[i]] = true;
        dpy[i+1][j-vertical[i]] = true;
      }
    }
  }
  if (!dpy[vertical.size()][10000+y]) {
    cout << "No" << endl;
    return 0;
  }

  cout << "Yes" << endl;

  return 0;
}
