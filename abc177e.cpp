#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>

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
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  unordered_set<ll> primeFound;
  bool isPairwiseCoprime = true;
  for (int i = 0; i < n; i++) {
    auto primeFactor = primeFactorize(a[i]);
    for (auto &x: primeFactor) {
      if (x.first == 1) continue;
      else if (primeFound.find(x.first) != primeFound.end()) {
        isPairwiseCoprime = false;
        break;
      }
      else primeFound.insert(x.first);
    }
    if (!isPairwiseCoprime) break;
  }

  if (isPairwiseCoprime) {
    cout << "pairwise coprime" << endl;
    return 0;
  }

  ll gcdAll = a[0];
  for (int i = 1; i < n; i++) gcdAll = gcd(gcdAll, a[i]);

  if (gcdAll == 1) cout << "setwise coprime" << endl;
  else cout << "not coprime" << endl;

  return 0;
}
