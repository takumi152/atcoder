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

vector<vector<ll> > matmulmod(vector<vector<ll> > &a, vector<vector<ll> > &b, ll m = mod) {
  vector<vector<ll> > ans(a.size(), vector<ll>(b[0].size()));
  for (int i = 0; i < (int) a.size(); i++) {
    for (int j = 0; j < (int) b[0].size(); j++) {
      for (int k = 0; k < (int) a[i].size(); k++) ans[i][j] = (ans[i][j] + a[i][k] * b[k][j]) % m;
      if (ans[i][j] < 0) ans[i][j] += m;
    }
  }

  return ans;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll h, w;
  cin >> h >> w;

  vector<vector<ll> > factor(1 << h, vector<ll>(1 << h));

  auto search = [&](auto self, int next, int now_state, int next_state) {
    if (next == h) {
      factor[now_state][next_state]++;
      return;
    }
    else if (next > h) return;

    self(self, next+1, now_state, next_state); // unplaceable
    self(self, next+1, now_state | (1 << next), next_state); // hanjo
    if (next+2 <= h) self(self, next+2, now_state | (1 << next) | (1 << (next+1)), next_state); // tatami, vertical
    self(self, next+1, now_state | (1 << next), next_state & ~(1 << next)); // tatami, horizontal
  };

  search(search, 0, 0, (1 << h) - 1);

  vector<vector<ll> > state_count(1 << h, vector<ll>(1));
  state_count[(1 << h) - 1][0] = 1;
  ll next = 1;
  while (next <= w) {
    if (w & next) state_count = matmulmod(factor, state_count);
    factor = matmulmod(factor, factor);
    next <<= 1;
  }

  ll ans = state_count[(1 << h) - 1][0];

  cout << ans << endl;

  return 0;
}
