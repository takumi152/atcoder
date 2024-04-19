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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, s;
  cin >> n >> s;
  vector<int> a(n), b(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

  vector<vector<int>> dp(n+1, vector<int>(s+1));
  dp[0][0] = 1; // 1 = H, 2 = T
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < s; j++) {
      if (dp[i][j] != 0) {
        if (j + a[i] <= s) dp[i+1][j+a[i]] = 1;
        if (j + b[i] <= s) dp[i+1][j+b[i]] = 2;
      }
    }
  }

  if (dp[n][s] != 0) {
    string ans = "";
    int rem = s;
    for (int i = n-1; i >= 0; i--) {
      if (dp[i+1][rem] == 1) {
        ans += "H";
        rem -= a[i];
      }
      else {
        ans += "T";
        rem -= b[i];
      }
    }
    reverse(ans.begin(), ans.end());

    cout << "Yes" << endl;
    cout << ans << endl;
  }
  else {
    cout << "No" << endl;
  }

  return 0;
}
