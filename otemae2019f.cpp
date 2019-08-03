#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <iterator>
#include <set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, D;
  cin >> N >> D;
  vector<vector<ll> > M(D, vector<ll>(N));
  for (int i = 0; i < D; i++) {
    for (auto &x: M[i]) cin >> x;
  }

  vector<vector<ll> > G(D, vector<ll>(N+1));
  for (int i = 0; i < D; i++) {
    ll sum = 0;
    for (auto &x: M[i]) sum += x;
    ll part_sum = 0;
    G[i][0] = sum;
    for (int j = 1; j <= N; j++) {
      part_sum += M[i][j-1];
      G[i][j] = abs(sum - part_sum * 2);
    }
  }

  vector<vector<ll> > dp(D, vector<ll>(N+1));
  for (int j = 0; j <= N; j++) dp[0][j] = G[0][j];
  for (int i = 1; i < D; i++) {
    int currentSmallest = 0;
    for (int j = 0; j <= N; j++) {
      if (dp[i-1][j] < dp[i-1][currentSmallest]) currentSmallest = j;
      dp[i][j] = dp[i-1][currentSmallest] + G[i][j];
    }
  }

  ll smallest = (ll)9e18;
  for (int j = 0; j <= N; j++) {
    if (dp[D-1][j] < smallest) smallest = dp[D-1][j];
  }

  cout << smallest << endl;

  return 0;
}
