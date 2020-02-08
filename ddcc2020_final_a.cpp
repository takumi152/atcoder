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
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  vector<unordered_map<ll, int> > factor(n);
  for (int i = 0; i < n; i++) factor[i] = primeFactorize(a[i]);

  vector<int> factorCount(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: factor[i]) {
      if (x.first != 1) factorCount[i] += x.second;
    }
  }

  int grundy = 0;
  for (int i = 0; i < n; i++) {
    grundy ^= factorCount[i];
  }

  if (grundy == 0) {
    cout << "No" << endl;
  }
  else {
    cout << "Yes" << endl;
  }

  return 0;
}
