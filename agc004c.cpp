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

  vector<vector<char> > r(h, vector<char>(w, '.'));
  vector<vector<char> > b(h, vector<char>(w, '.'));

  for (int i = 1; i < h-1; i++) {
    r[i][0] = '#';
    b[i][w-1] = '#';
    for (int j = 1; j < w-1; j++) {
      if (i % 2 == 0) {
        r[i][j] = '#';
        if (s[i][j] == '#') b[i][j] = '#';
      }
      else {
        if (s[i][j] == '#') r[i][j] = '#';
        b[i][j] = '#';
      }
    }
  }

  for (auto &x: r) {
    for (auto &y: x) cout << y;
    cout << endl;
  }
  cout << endl;
  for (auto &x: b) {
    for (auto &y: x) cout << y;
    cout << endl;
  }

  return 0;
}
