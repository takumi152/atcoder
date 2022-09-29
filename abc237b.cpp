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
  vector<vector<int> > a(h, vector<int>(w));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }

  for (int j = 0; j < w; j++) {
    for (int i = 0; i < h; i++) cout << a[i][j] << " ";
    cout << endl;
  }

  return 0;
}
