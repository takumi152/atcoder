#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, K;
  cin >> N >> K;
  vector<int> h(N);
  for (auto &x: h) cin >> x;

  vector<int> dp(N+2);
  dp[1] = abs(h[1] - h[0]);
  for (int i = 2; i < N; i++) {
    int minimum = 1000000007;
    for (int j = i - K; j < i; j++) {
      if (j < 0) continue;
      int x = dp[j] + abs(h[i] - h[j]);
      if (x < minimum) minimum = x;
    }
    dp[i] = minimum;
  }

  cout << dp[N-1] << endl;
  return 0;
}
