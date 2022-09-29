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

unordered_map<int, int> prime_factorize(int a) {
  int i = 2;
  unordered_map<int, int> factor;
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

  int n, m;
  cin >> n >> m;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  unordered_set<int> prime_factor;
  for (int i = 0; i < n; i++) {
    auto factor = prime_factorize(a[i]);
    for (auto &[f, _]: factor) {
      if (f != 1) prime_factor.insert(f);
    }
  }

  vector<bool> coprime(m+1, true);
  coprime[0] = false;

  for (auto &f: prime_factor) {
    for (int i = f; i <= m; i += f) coprime[i] = false;
  }

  vector<int> ans;
  for (int i = 1; i <= m; i++) {
    if (coprime[i]) ans.push_back(i);
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x << endl;

  return 0;
}
