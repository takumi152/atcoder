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

  int n;
  cin >> n;
  vector<ll> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  a.push_back(b[0]);
  b.push_back(b[0]);
  a.insert(a.begin(), b[n-1]);
  b.insert(b.begin(), b[n-1]);

  priority_queue<pair<ll, int> > que;
  for (int i = 1; i < n+1; i++) que.emplace(b[i], i);
  ll ans = 0;
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (now.first == a[now.second]) continue;
    if (now.first - (b[now.second - 1] + b[now.second + 1]) < a[now.second]) {
      cout << -1 << endl; // impossible
      return 0;
    }
    ans += (b[now.second] - max(max(b[now.second - 1], b[now.second + 1]), a[now.second]) + (b[now.second - 1] + b[now.second + 1]) - 1) / (b[now.second - 1] + b[now.second + 1]);
    b[now.second] -= (b[now.second - 1] + b[now.second + 1]) * ((b[now.second] - max(max(b[now.second - 1], b[now.second + 1]), a[now.second]) + (b[now.second - 1] + b[now.second + 1]) - 1) / (b[now.second - 1] + b[now.second + 1]));
    if (now.second == 1) b[n+1] = b[1];
    else if (now.second == n) b[0] = b[n];
    que.emplace(b[now.second], now.second);
  }

  cout << ans << endl;

  return 0;
}
