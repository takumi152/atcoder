// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, k;
  cin >> n >> k;

  if (k % 2 == 1) {
    cout << 0 << endl; // impossible
    return 0;
  }

  vector<ll> factorial;
  factorial.push_back(0); // 0! = 0
  for (ll i = 1; i < 2 * n; i++) factorial.push_back((factorial[i-1] * i) % MOD); // i! = (i-1)! * i
  vector<vector<ll> > dp(N+2, vector<ll>(N * N + 2));
  dp[0][0] = 1;
  for (int i = 1; i < N; i++) {
    for (int j = 0; j < N * N; j++) {

    }
  }

  return 0;
}
