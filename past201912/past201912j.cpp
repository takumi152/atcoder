// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<vector<ll> > a(h, vector<ll>(w));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }

  ll best = inf;
  priority_queue<pair<ll, unordered_set<Pii> >, vector<pair<ll, unordered_set<Pii> > >, greater<pair<ll, unordered_set<Pii> > > > que;
  que.emplace(0, unordered_set<Pii>({Pii(h-1, 0)}));
  while (!que.empty()) {
    auto now = que.top();
    que.pop();

  }

  return 0;
}
