#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll N, K;
  cin >> N >> K;
  vector<ll> A(N);
  for (auto &x: A) cin >> x;

  ll ans = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (A[i] > A[j]) {
        if (i < j) ans = (ans+(K*(K+1))/2) % MOD;
        else ans = (ans+((K*(K+1))/2)-K) % MOD;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
