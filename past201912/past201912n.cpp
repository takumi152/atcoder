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
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, w, c;
  cin >> n >> w >> c;
  vector<vector<ll> > lrc(n, vector<ll>(3));
  for (auto &x: lrc) {
    for (auto &y: x) cin >> y;
  }

  sort(lrc.begin(), lrc.end());

  int p = 0;
  ll cost = 0;
  ll best = inf;
  priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > que;
  while (p < n) {
    if (lrc[p][0] < c) {
      cost += lrc[p][2];
      que.emplace(lrc[p][1], p);
      p++;
    }
    else break;
  }
  best = cost;
  while (p < n) {
    while (!que.empty()) {
      auto now = que.top();
      if (now.first <= lrc[p][0] - c) {
        cost -= lrc[now.second][2];
        que.pop();
      }
      else break;
    }
    if (cost < best) best = cost;
    cost += lrc[p][2];
    que.emplace(lrc[p][1], p);
    p++;
  }
  while (!que.empty()) {
    auto now = que.top();
    if (now.first <= w - c) {
      cost -= lrc[now.second][2];
      que.pop();
    }
    else break;
  }
  if (cost < best) best = cost;

  cout << best << endl;

  return 0;
}
