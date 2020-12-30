#include <iostream>
#include <iomanip>
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

vector<ll> enumerateDivisible(ll n) {
  vector<ll> divisible;
  for (ll i = 1; i * i <= n; i++) {
    if (n % i == 0) {
      divisible.push_back(i);
      if (i * i != n) divisible.push_back(n / i);
    }
  }

  sort(divisible.begin(), divisible.end());
  return divisible;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll s, p;
  cin >> s >> p;

  auto divisible = enumerateDivisible(p);

  int n = divisible.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (divisible[i] + divisible[j] == s && divisible[i] * divisible[j] == p) {
        cout << "Yes" << endl;
        return 0;
      }
    }
  }

  cout << "No" << endl;

  return 0;
}
