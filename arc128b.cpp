#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <cstdlib>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<vector<int> > rgb(t, vector<int>(3));
  for (auto &x: rgb) {
    for (auto &y: x) cin >> y;
  }

  vector<int> ans(t, -1);
  for (int i = 0; i < t; i++) {
    sort(rgb[i].begin(), rgb[i].end());
    if ((rgb[i][1] - rgb[i][0]) % 3 == 0) ans[i] = rgb[i][1];
    else if ((rgb[i][2] - rgb[i][0]) % 3 == 0) ans[i] = rgb[i][2];
    else if ((rgb[i][2] - rgb[i][1]) % 3 == 0) ans[i] = rgb[i][2];
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
