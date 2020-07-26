#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("sse4.2")

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

int n;
vector<ll> x, y, p, ans;
vector<vector<ll> > m;

void calcScore(int next, int count) {
  if (next < n) {
    for (int i = 0; i < n; i++) m[next+1][i] = m[next][i];
    calcScore(next+1, count);
    for (int i = 0; i < n; i++) {
      m[next+1][i] = m[next][i];
      if (abs(x[i] - x[next]) < m[next+1][i]) m[next+1][i] = abs(x[i] - x[next]);
    }
    calcScore(next+1, count+1);
    for (int i = 0; i < n; i++) {
      m[next+1][i] = m[next][i];
      if (abs(y[i] - y[next]) < m[next+1][i]) m[next+1][i] = abs(y[i] - y[next]);
    }
    calcScore(next+1, count+1);
  }
  else {
    ll score = 0;
    for (int i = 0; i < n; i++) score += p[i] * m[n][i];
    if (score < ans[count]) ans[count] = score;
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  cin >> n;
  x = vector<ll>(n);
  y = vector<ll>(n);
  p = vector<ll>(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> p[i];

  m = vector<vector<ll> >(n+1, vector<ll>(n));
  for (int i = 0; i < n; i++) m[0][i] = min(abs(x[i]), abs(y[i]));

  ans = vector<ll>(n+1, (ll) 9e18);
  calcScore(0, 0);

  for (auto &z: ans) cout << z << endl;

  return 0;
}
