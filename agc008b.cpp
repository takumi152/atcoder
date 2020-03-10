#include <iostream>
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

  int n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<ll> a_sum(n+1), a_sum_optimal(n+1);
  for (int i = 0; i < n; i++) {
    a_sum[i+1] = a_sum[i] + a[i];
    if (a[i] > 0) a_sum_optimal[i+1] = a_sum_optimal[i] + a[i];
    else a_sum_optimal[i+1] = a_sum_optimal[i];
  }

  ll ans = 0;
  for (int i = 0; i < n - k + 1; i++) {
    ll white = (a_sum_optimal[i] - a_sum_optimal[0]) + (a_sum_optimal[n] - a_sum_optimal[i+k]);
    ll black = (a_sum_optimal[i] - a_sum_optimal[0]) + (a_sum[i+k] - a_sum[i]) + (a_sum_optimal[n] - a_sum_optimal[i+k]);
    if (max(white, black) > ans) ans = max(white, black);
  }

  cout << ans << endl;

  return 0;
}
