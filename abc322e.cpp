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

struct Plan {
  ll cost;
  vector<int> param_increase;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k, p;
  cin >> n >> k >> p;
  vector<Plan> plans(n);
  for (int i = 0; i < n; i++) {
    plans[i].param_increase = vector<int>(k);
    cin >> plans[i].cost;
    for (auto &x: plans[i].param_increase) cin >> x;
    plans[i].param_increase.resize(5);
  }

  vector<int> target_param(5);
  for (int i = 0; i < k; i++) target_param[i] = p;

  vector<vector<vector<vector<vector<ll>>>>> dp(p + 1, vector<vector<vector<vector<ll>>>>(p + 1, vector<vector<vector<ll>>>(p + 1, vector<vector<ll>>(p + 1, vector<ll>(p + 1, (ll) 1e18)))));
  dp[0][0][0][0][0] = 0;
  for (int i = 0; i < n; i++) {
    auto &pi = plans[i].param_increase;
    for (int p0 = p; p0 >= 0; p0--) {
      for (int p1 = p; p1 >= 0; p1--) {
        for (int p2 = p; p2 >= 0; p2--) {
          for (int p3 = p; p3 >= 0; p3--) {
            for (int p4 = p; p4 >= 0; p4--) {
              auto &base = dp[p0][p1][p2][p3][p4];
              auto &target = dp[min(target_param[0], p0 + pi[0])][min(target_param[1], p1 + pi[1])][min(target_param[2], p2 + pi[2])][min(target_param[3], p3 + pi[3])][min(target_param[4], p4 + pi[4])];
              target = min(target, base + plans[i].cost);
            }
          }
        }
      }
    }
  }

  ll ans = dp[target_param[0]][target_param[1]][target_param[2]][target_param[3]][target_param[4]];
  if (ans == (ll) 1e18) cout << -1 << endl;
  else cout << ans << endl;

  return 0;
}
