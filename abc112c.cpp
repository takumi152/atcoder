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

  int n;
  cin >> n;
  vector<vector<int> > xyh(n, vector<int>(3));
  for (auto &x: xyh) {
    for (auto &y: x) cin >> y;
  }

  sort(xyh.rbegin(), xyh.rend(), [](auto &a, auto &b){return a[2] < b[2];});

  for (int i = 0; i <= 100; i++) {
    for (int j = 0; j <= 100; j++) {
      bool good = true;
      int h = xyh[0][2] + abs(i - xyh[0][0]) + abs(j - xyh[0][1]);
      for (int k = 1; k < n; k++) {
        if (max(h - abs(i - xyh[k][0]) - abs(j - xyh[k][1]), 0) != xyh[k][2]) {
          good = false;
          break;
        }
      }
      if (good) {
        cout << i << " " << j << " " << h << endl;
        return 0;
      }
    }
  }

  return 0;
}
