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

  int n, l;
  cin >> n >> l;
  vector<bool> h(l+1);
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    h[x] = true;
  }
  int t1, t2, t3;
  cin >> t1 >> t2 >> t3;

  vector<int> dp(l+1, 1000000007);
  dp[0] = 0;
  for (int i = 0; i < l; i++) {
    dp[i+1] = min(dp[i+1], dp[i] + t1 + (h[i+1] ? t3 : 0));
    if (i+2 > l) dp[l] = min(dp[l], dp[i] + t1 / 2 + (t2 / 2) * ((l - i) * 2 - 1));
    else dp[i+2] = min(dp[i+2], dp[i] + t1 + t2 + (h[i+2] ? t3 : 0));
    if (i+4 > l) dp[l] = min(dp[l], dp[i] + t1 / 2 + (t2 / 2) * ((l - i) * 2 - 1));
    else dp[i+4] = min(dp[i+4], dp[i] + t1 + t2 * 3 + (h[i+4] ? t3 : 0));
  }

  cout << dp[l] << endl;

  return 0;
}
