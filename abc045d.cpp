#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll h, w, n;
  cin >> h >> w >> n;
  set<Pii> ab;
  for (int i = 0; i < n; i++) {
    Pii buf;
    cin >> buf.first >> buf.second;
    ab.insert(buf);
  }

  vector<set<Pii> > found(10);
  ll base = (h-2) * (w-2);

  for (auto &x: ab) {
    for (int i = x.first - 1; i <= x.first + 1; i++) {
      for (int j = x.second - 1; j <= x.second + 1; j++) {
        bool good = true;
        int count = 0;
        for (int k = i - 1; k <= i + 1; k++) {
          for (int l = j - 1; l <= j + 1; l++) {
            if (k <= 0 || k > h || l <= 0 || l > w) {
              good = false;
              break;
            }
            if (ab.find(Pii(k, l)) != ab.end()) {
              count++;
            }
          }
          if (!good) break;
        }
        if (good) found[count].insert(Pii(i, j));
      }
    }
  }

  vector<ll> ans(10);
  ans[0] = base;
  for (int i = 1; i <= 9; i++) {
    ans[i] = found[i].size();
    ans[0] -= ans[i];
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
