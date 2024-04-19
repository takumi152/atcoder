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

  int h, w;
  cin >> h >> w;
  vector<vector<char>> c(h, vector<char>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) cin >> c[i][j];
  }

  vector<int> ans(w);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
        if (c[i][j] == '#') ans[j]++;
    }
  }

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
