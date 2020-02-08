#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

unordered_map<ll, int> primeFactorize(ll a) {
  ll i = 2;
  unordered_map<ll, int> factor;
  while (i * i <= a) {
    if (a % i == 0) {
      factor[i]++;
      a /= i;
    }
    else i++;
  }
  factor[a]++;
  return factor;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  unordered_map<ll, int> totalFactor;
  for (int i = 2; i <= n; i++) {
    auto factor = primeFactorize(i);
    for (auto &x: factor) totalFactor[x.first] += x.second;
  }

  ll ans = 1;
  for (auto &x: totalFactor) ans = (ans * (x.second + 1)) % mod;

  cout << ans << endl;

  return 0;
}
