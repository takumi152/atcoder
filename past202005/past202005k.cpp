#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<int> f(q), t(q), x(q);
  for (int i = 0; i < q; i++) {
    cin >> f[i] >> t[i] >> x[i];
    f[i]--;
    t[i]--;
    x[i]--;
  }

  vector<int> next(n);
  vector<int> top(n);
  for (int i = 0; i < n; i++) {
    next[i] = i+n;
    top[i] = i;
  }

  for (int i = 0; i < q; i++) {
    if (next[x[i]] < n) {
      int ti = top[t[i]];
      top[t[i]] = top[f[i]];
      top[f[i]] = next[x[i]];
      if (ti == -1) next[x[i]] = f[i]+n;
      else next[x[i]] = ti;
    }
    else {
      int ti = top[t[i]];
      top[t[i]] = top[f[i]];
      top[f[i]] = -1;
      if (ti == -1) next[x[i]] = f[i]+n;
      else next[x[i]] = ti;
    }
  }

  vector<int> ans(n, -1);
  for (int i = 0; i < n; i++) {
    if (top[i] == -1) continue;
    int now = top[i];
    while (true) {
      ans[now] = i+1;
      now = next[now];
      if (now >= n) break;
    }
  }

  for (auto &y: ans) cout << y << endl;

  return 0;
}
