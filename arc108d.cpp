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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  char c_aa, c_ab, c_ba, c_bb;
  cin >> c_aa;
  cin >> c_ab;
  cin >> c_ba;
  cin >> c_bb;

  if ((c_ab == 'A' && c_aa == 'A') || (c_ab == 'B' && c_bb == 'B') || n <= 3) {
    cout << 1 << endl;
    return 0;
  }

  int a_lower_limit_first = 0;
  int a_upper_limit_first = n;
  int a_lower_limit = 0;
  int a_upper_limit = n;
  if (c_ab == 'A' && c_aa == 'B') {
    a_lower_limit_first = 1;
    a_upper_limit_first = n;
    a_lower_limit = 1;
    a_upper_limit = n;
    if (c_ba == 'B') {
      a_lower_limit = 0;
    }
  }
  else if (c_ab == 'B' && c_bb == 'A') {
    a_lower_limit_first = 1;
    a_upper_limit_first = 1;
    a_lower_limit = 0;
    a_upper_limit = 1;
    if (c_ba == 'A') {
      a_upper_limit = n;
    }
  }

  vector<ll> dp(n+1);
  dp[0] = 1;
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      for (int j = a_lower_limit_first; j <= a_upper_limit_first && i + j + 1 <= n; j++) {
        dp[i+j+1] = (dp[i+j+1] + dp[i]) % mod;
      }
    }
    else if (c_ab == 'A' && i == n-1) continue;
    else {
      for (int j = a_lower_limit; j <= a_upper_limit && i + j + 1 <= n; j++) {
        dp[i+j+1] = (dp[i+j+1] + dp[i]) % mod;
      }
    }
  }

  cout << dp[n] << endl;

  return 0;
}
