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

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '.') continue;
      if (i > 0) {
        if (s[i-1][j] == '#') continue;
      }
      if (i < h-1) {
        if (s[i+1][j] == '#') continue;
      }
      if (j > 0) {
        if (s[i][j-1] == '#') continue;
      }
      if (j < h-1) {
        if (s[i][j+1] == '#') continue;
      }
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
