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

  int n, k;
  cin >> n >> k;
  vector<pair<int, ll> > td(n);
  for (auto &x: td) cin >> x.first >> x.second;

  vector<vector<ll> > c(n);
  for (int i = 0; i < n; i++) {
    c[td[i].first-1].push_back(td[i].second);
  }

  for (int i = 0; i < n; i++) sort(c[i].rbegin(), c[i].rend());

  ll ans = 0;
  ll varCount = 0;
  priority_queue<ll, vector<ll>, greater<ll> > noPenalty, penalty;
  for (int i = 0; i < n; i++) {
    if (c[i].size() == 0) continue;
    varCount++;
    penalty.push(c[i][0]);
    for (int j = 1; j < (int) c[i].size(); j++) noPenalty.push(c[i][j]);
    for (auto &x: c[i]) ans += x;
  }
  ans += varCount * varCount;
  for (int i = n; i > k; i--) {
    ll p, np;
    p = penalty.top();
    if (!noPenalty.empty()) np = noPenalty.top();
    else np = 9e18;
    if (p + 2 * varCount - 1 <= np) {
      penalty.pop();
      ans -= p + 2 * varCount - 1;
      varCount--;
    }
    else {
      noPenalty.pop();
      ans -= np;
    }
  }

  cout << ans << endl;

  return 0;
}
