#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

struct Query {
  char op;
  int x;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int q, k;
  cin >> q >> k;
  vector<Query> queries(q);
  for (auto &x: queries) {
    cin >> x.op >> x.x;
  }

  vector<ll> ans(q);
  vector<ll> dp(k+1);
  dp[0] = 1;
  for (int i = 0; i < q; i++) {
    auto query = queries[i];
    if (query.op == '+') {
      for (int j = k; j - query.x >= 0; j--) {
        dp[j] = (dp[j] + dp[j - query.x]) % mod;
      }
    }
    else {
      for (int j = 0; j + query.x <= k; j++) {
        dp[j + query.x] = (dp[j + query.x] - dp[j]) % mod;
        if (dp[j + query.x] < 0) dp[j + query.x] += mod;
      }
    }
    ans[i] = dp[k];
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
