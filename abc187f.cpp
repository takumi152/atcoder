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

vector<bool> is_clique;
vector<bool> is_clique_checked;
vector<int> dp;

bool check_clique(int now, int n, vector<vector<bool> > &edge) {
  if (is_clique_checked[now]) return is_clique[now];

  is_clique_checked[now] = true;

  for (int i = 0; i < n; i++) {
    if (!(now & (1 << i))) continue;
    for (int j = i+1; j < n; j++) {
      if (!(now & (1 << j))) continue;
      if (!edge[i][j]) return false;
    }
  }

  is_clique[now] = true;
  return true;
}

int solve(int now, int n, vector<vector<bool> > &edge) {
  if (dp[now] != -1) return dp[now];

  if (now == 0) {
    dp[now] = 0;
    return dp[now];
  }

  dp[now] = 1000000007;

  for (int next = now; next > 0; next = (next - 1) & now) {
    if (check_clique(next, n, edge)) {
      dp[now] = min(dp[now], solve(now & ~(next), n, edge) + 1);
    }
  }

  return dp[now];
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<bool> > edge(n, vector<bool>(n));
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1][b-1] = true;
    edge[b-1][a-1] = true;
  }

  is_clique = vector<bool>(1 << n);
  is_clique_checked = vector<bool>(1 << n);
  dp = vector<int>(1 << n, -1);
  int ans = solve((1 << n) - 1, n, edge);

  cout << ans << endl;

  return 0;
}
