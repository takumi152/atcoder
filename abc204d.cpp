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
  vector<int> t(n);
  for (auto &x: t) cin >> x;

  int total_time = 0;
  for (int i = 0; i < n; i++) total_time += t[i];

  int target_time = total_time / 2;
  vector<bool> dp(target_time+1);
  dp[0] = true;
  for (int i = 0; i < n; i++) {
    for (int j = target_time; j >= 0; j--) {
      if (dp[j] && j + t[i] <= target_time) dp[j + t[i]] = true;
    }
  }

  int best_time = 0;
  for (int i = 0; i <= target_time; i++) {
    if (dp[i]) best_time = i;
  }


  int ans = total_time - best_time;

  cout << ans << endl;

  return 0;
}
