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

  int x, y, a, b, c;
  cin >> x >> y >> a >> b >> c;
  vector<ll> p(a), q(b), r(c);
  for (auto &z: p) cin >> z;
  for (auto &z: q) cin >> z;
  for (auto &z: r) cin >> z;

  sort(p.rbegin(), p.rend());
  sort(q.rbegin(), q.rend());
  sort(r.rbegin(), r.rend());

  priority_queue<ll, vector<ll>, greater<ll> > pque, qque;
  for (int i = 0; i < x; i++) pque.push(p[i]);
  for (int i = 0; i < y; i++) qque.push(q[i]);
  for (int i = 0; i < c; i++) {
    ll pmin = pque.top();
    ll qmin = qque.top();
    if (pmin < qmin) {
      if (pmin < r[i]) {
        pque.pop();
        pque.push(r[i]);
      }
      else break;
    }
    else {
      if (qmin < r[i]) {
        qque.pop();
        qque.push(r[i]);
      }
      else break;
    }
  }

  ll ans = 0;
  while (!pque.empty()) {
    ll now = pque.top();
    pque.pop();
    ans += now;
  }
  while (!qque.empty()) {
    ll now = qque.top();
    qque.pop();
    ans += now;
  }

  cout << ans << endl;

  return 0;
}
