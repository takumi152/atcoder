#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

void recurse(int prev, int now, ll base, vector<unordered_map<int, ll> > &edge, vector<ll> &ans) {
  base += edge[now][prev];
  base -= edge[prev][now];
  ans[now] = base;
  for (auto &[next, weight]: edge[now]) {
    if (next == prev) continue;
    recurse(now, next, base, edge, ans);
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Pii> ab(n-1);
  for (auto &x: ab) cin >> x.first >> x.second;
  int q;
  cin >> q;
  vector<vector<ll> > tex(q, vector<ll>(3));
  for (auto &x: tex) cin >> x[0] >> x[1] >> x[2];

  vector<unordered_map<int, ll> > edge(n);
  for (auto &x: ab) {
    edge[x.first-1][x.second-1] = 0;
    edge[x.second-1][x.first-1] = 0;
  }

  for (int i = 0; i < q; i++) {
    if (tex[i][0] == 1) edge[ab[tex[i][1]-1].first-1][ab[tex[i][1]-1].second-1] += tex[i][2];
    else edge[ab[tex[i][1]-1].second-1][ab[tex[i][1]-1].first-1] += tex[i][2];
  }

  ll base = 0;
  stack<Pii> st;
  st.emplace(0, 0);
  while (!st.empty()) {
    auto [prev, now] = st.top();
    st.pop();
    for (auto &[next, weight]: edge[now]) {
      if (next == prev) continue;
      base += weight;
      st.emplace(now, next);
    }
  }

  vector<ll> ans(n);
  recurse(0, 0, base, edge, ans);

  for (auto &x: ans) cout << x << endl;

  return 0;
}
