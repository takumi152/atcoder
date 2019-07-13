#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> h(N);
  for (auto &x: h) cin >> x;

  vector<int> dp(N+2);
  dp[1] = abs(h[1] - h[0]);
  for (int i = 2; i < N; i++) {
    dp[i] = min(dp[i-1] + abs(h[i] - h[i-1]), dp[i-2] + abs(h[i] - h[i-2]));
  }

  cout << dp[N-1] << endl;
  return 0;
}
