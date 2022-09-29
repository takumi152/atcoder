#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  vector<ll> dp(n);
  dp[0] = 1;
  vector<int> fastest(n, 1000000007);
  fastest[0] = 0;
  queue<int> que;
  que.push(0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    for (auto &next: edge[now]) {
      if (fastest[next] == 1000000007) {
        dp[next] = dp[now];
        fastest[next] = fastest[now] + 1;
        que.push(next);
      }
      else if (fastest[next] == fastest[now] + 1) {
        dp[next] = (dp[next] + dp[now]) % mod;
      }
    }
  }

  cout << dp[n-1] << endl;

  return 0;
}
