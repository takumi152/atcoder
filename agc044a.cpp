#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<ll> n(t), a(t), b(t), c(t), d(t);
  for (int i = 0; i < t; i++) cin >> n[i] >> a[i] >> b[i] >> c[i] >> d[i];

  vector<ll> ans(t, (ll) 9e18);
  for (int i = 0; i < t; i++) {
    priority_queue<pair<ll, ll>, vector<pair<ll, ll> >, greater<pair<ll, ll> > > que;
    unordered_set<ll> visited;
    que.emplace(0, n[i]);
    while (!que.empty()) {
      auto now = que.top();
      que.pop();
      if (visited.find(now.second) != visited.end()) continue;
      visited.insert(now.second);
      if (now.second == 0) {
        ans[i] = now.first;
        break;
      }
      for (ll j = -5; j <= 5; j++) {
        if (now.second + j < 0) continue;
        if (now.second + j == 0) {
          que.emplace(now.first + d[i] * abs(j), 0);
          continue;
        }
        if ((now.second + j) % 2 == 0) {
          if (abs(now.second - ((now.second + j) / 2)) * d[i] < d[i] * abs(j) + a[i] && now.second < (ll) 2e9) que.emplace(now.first + abs(now.second - ((now.second + j) / 2)) * d[i], (now.second + j) / 2);
          else que.emplace(now.first + d[i] * abs(j) + a[i], (now.second + j) / 2);
        }
        if ((now.second + j) % 3 == 0) {
          if (abs(now.second - ((now.second + j) / 3)) * d[i] < d[i] * abs(j) + b[i] && now.second < (ll) 2e9) que.emplace(now.first + abs(now.second - ((now.second + j) / 3)) * d[i], (now.second + j) / 3);
          else que.emplace(now.first + d[i] * abs(j) + b[i], (now.second + j) / 3);
        }
        if ((now.second + j) % 5 == 0) {
          if (abs(now.second - ((now.second + j) / 5)) * d[i] < d[i] * abs(j) + c[i] && now.second < (ll) 2e9) que.emplace(now.first + abs(now.second - ((now.second + j) / 5)) * d[i], (now.second + j) / 5);
          else que.emplace(now.first + d[i] * abs(j) + c[i], (now.second + j) / 5);
        }
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
