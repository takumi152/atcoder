// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <utility>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<Pii> xy(N);
  for(auto &a: xy) cin >> a.first >> a.second;

  sort(xy.begin(), xy.end());
  vector<ll> dp(N+2);
  dp[0] = 1;
  dp[1] = 4;
  for (int i = 2; i < N; i++) {

  }

  return 0;
}
