// WA

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
  string s;
  cin >> s;

  vector<bool> a(1 << n);
  for (int i = 0; i < (1 << n); i++) a[i] = s[i] - '0';

  for (int i = 0; i < (1 << n); i++) {
    bool good = true;
    for (int j = 0; j < (1 << n); j++) {
      bool obscured = false;
      for (int k = 0; k < n; k++) {
        bool point = j & (1 << k);
        if (point && (i & (1 << k))) {
          obscured = true;
          break;
        }
      }
      if (obscured == a[j]) {
        good = false;
        break;
      }
    }
    if (good) {
      vector<Pii> loop;
      bool up = false;
      for (int j = 0; j < n; j++) {
        if (!up) {
          loop.emplace_back(j, 0);
          if (i & (1 << j)) {
            loop.emplace_back(j, 1);
            up = true;
          }
        }
        else {
          loop.emplace_back(j, 1);
          if (!(i & (1 << j))) {
            loop.emplace_back(j, 0);
            up = false;
          }
        }
      }
      if (up) loop.emplace_back(n, 1);
      for (int j = n; j >= 0; j--) loop.emplace_back(j, 0);

      cout << "Possible" << endl;
      cout << loop.size() - 1 << endl;
      for (auto &x: loop) cout << x.first << " " << x.second << endl;
      return 0;
    }
  }

  cout << "Impossible" << endl;

  return 0;
}
