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

  int k;
  cin >> k;

  priority_queue<ll, vector<ll>, greater<ll> > pque;
  for (int i = 1; i <= 9; i++) pque.push(i);
  int p = 0;
  ll ans = 0;
  while (!pque.empty()) {
    auto now = pque.top();
    pque.pop();
    p++;
    if (p == k) {
      ans = now;
      break;
    }
    if (now % 10 > 0) pque.push(now * 10 + now % 10 - 1);
    pque.push(now * 10 + now % 10);
    if (now % 10 < 9) pque.push(now * 10 + now % 10 + 1);
  }

  cout << ans << endl;

  return 0;
}
