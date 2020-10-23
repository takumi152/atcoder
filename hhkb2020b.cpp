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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  int ans = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (i < h-1) {
        if (s[i][j] == '.' && s[i+1][j] == '.') ans++;
      }
      if (j < w-1) {
        if (s[i][j] == '.' && s[i][j+1] == '.') ans++;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
