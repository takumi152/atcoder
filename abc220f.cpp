#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<map<ll, vector<ll> > > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1][b-1] = vector<ll>({-1, -1});
    edge[b-1][a-1] = vector<ll>({-1, -1});
  }

  vector<ll> remaining_edge(n);
  for (int i = 0; i < n; i++) remaining_edge[i] = edge[i].size();
  vector<ll> total_ans(n);

  auto calc_vertex = [&](auto self, int prev, int now) {
    if (false) return pair<ll, ll>();
    if (remaining_edge[now] == 0) {
      if (prev == -1) return pair<ll, ll>(total_ans[now], n - 1);
      else return pair<ll, ll>(total_ans[now] + (n - 1) - (edge[now][prev][0] + edge[now][prev][1]) + 1, (n - 1) - edge[now][prev][1]);
    }
    ll ans = 0;
    ll subtree_vertex = 1;
    for (auto &[next, info]: edge[now]) {
      if (next == prev) continue;
      else if (info[0] != -1) {
        ans += info[0];
        subtree_vertex += info[1];
      }
      else {
        auto [next_ans, next_subtree_vertex] = self(self, now, next);
        info[0] = next_ans;
        info[1] = next_subtree_vertex;
        ans += info[0];
        subtree_vertex += info[1];
        total_ans[now] += info[0];
        remaining_edge[now]--;
      }
    }
    if (prev != -1) ans += subtree_vertex;
    return pair<ll, ll>(ans, subtree_vertex);
  };

  vector<ll> ans(n);
  for (int i = 0; i < n; i++) {
    auto [res, _] = calc_vertex(calc_vertex, -1, i);
    ans[i] = res;
  }

  // for (auto &x: ans) cout << x << endl;
  // for (int i = 0; i < n; i++) {
  //   if (ans[i] != total_ans[i]) cerr << i << " " << ans[i] << " " << total_ans[i] << endl;
  // }
}
