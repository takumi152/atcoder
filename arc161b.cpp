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

  int t;
  cin >> t;
  vector<ll> n(t);
  for (auto &x: n) cin >> x;

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    bool good = false;
    for (int j = 62; j >= 0; j--) {
      if ((1LL << j) <= n[i]) {
        for (int k = j - 1; k >= 0; k--) {
          if ((1LL << j) + (1LL << k) <= n[i]) {
            for (int l = k - 1; l >= 0; l--) {
              if ((1LL << j) + (1LL << k) + (1LL << l) <= n[i])
              {
                good = true;
                ans[i] = (1LL << j) + (1LL << k) + (1LL << l);
                break;
              }
            }
          }
          if (good) break;
        }
      }
      if (good) break;
    }

    if (!good) ans[i] = -1;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
