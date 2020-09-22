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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, x, m;
  cin >> n >> x >> m;

  vector<bool> visited(m);
  vector<ll> loop;
  loop.push_back(x);
  visited[x] = true;
  ll now = x;
  while (true) {
    now = (now * now) % m;
    if (visited[now]) break;
    loop.push_back(now);
    visited[now] = true;
  }

  if ((int) loop.size() >= n) {
    ll ans = 0;
    for (int i = 0; i < n; i++) ans += loop[i];
    cout << ans << endl;
    return 0;
  }

  int loop_begin = 0;
  for (int i = 0; i < (int) loop.size(); i++) {
    if ((loop[loop.size()-1] * loop[loop.size()-1]) % m == loop[i]) {
      loop_begin = i;
      break;
    }
  }
  ll loop_size = loop.size() - loop_begin;

  ll loop_init = 0;
  for (int i = 0; i < loop_begin; i++) loop_init += loop[i];
  ll loop_sum = 0;
  for (int i = loop_begin; i < (int) loop.size(); i++) loop_sum += loop[i];

  if (loop[loop.size()-1] == 0) {
    cout << loop_init + loop_sum << endl;
    return 0;
  }

  ll ans = loop_init + loop_sum * ((n - loop_begin) / loop_size);
  ll rem = (n - loop_begin) % loop_size;
  for (int i = loop_begin; i < loop_begin + rem; i++) ans += loop[i];
  cout << ans << endl;

  return 0;
}
