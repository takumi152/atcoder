#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

vector<ll> erathosthenes(ll n) {
  vector<bool> mask(n+1, true);
  mask[0] = false;
  mask[1] = false;
  for (int i = 2; i * i <= n; i++) {
    for (int j = i * i; j <= n; j += i) {
      mask[j] = false;
    }
  }
  vector<ll> prime = {};
  for (int i = 0; i <= n; i++) {
    if (mask[i]) prime.push_back(i);
  }
  return prime;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, p;
  cin >> n >> p;

  vector<ll> prime = erathosthenes(p);

  vector<ll> prime_1, prime_2;
  for (int i = 0; i < (int) prime.size(); i++) {
    if (i % 2 == 0) prime_1.push_back(prime[i]);
    else prime_2.push_back(prime[i]);
  }

  auto dfs = [&](auto self, vector<ll>& res, ll now, const int depth, const vector<ll>& prime_container) {
    if (depth >= (int) prime_container.size()) return;
    for (int i = 0; ; i++) {
      if (i > 0) res.push_back(now);
      if (depth < (int) prime_container.size()-1) self(self, res, now, depth + 1, prime_container);
      now *= prime_container[depth];
      if (now > n) break;
    }
  };

  vector<ll> res_1 = {1};
  dfs(dfs, res_1, 1, 0, prime_1);
  vector<ll> res_2 = {1};
  dfs(dfs, res_2, 1, 0, prime_2);

  sort(res_1.begin(), res_1.end());
  sort(res_2.begin(), res_2.end());

  int p1 = 0;
  int p2 = res_2.size() - 1;
  ll ans = 0;
  while (p2 >= 0) {
    while (p1 < (int) res_1.size() && res_1[p1] * res_2[p2] <= n) {
      p1++;
    }
    ans += p1;
    p2--;
  }

  cout << ans << endl;

  return 0;
}
