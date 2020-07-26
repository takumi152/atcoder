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

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  ll maximum = 1000;
  vector<pair<ll, ll> > possibles;
  for (int i = 0; i < n; i++) {
    ll maximum_temp = maximum;
    for (auto [stock, rem]: possibles) {
      if (stock * a[i] + rem > maximum_temp) maximum_temp = stock * a[i] + rem;
    }
    possibles.emplace_back(maximum / a[i], maximum % a[i]);
    maximum = maximum_temp;
  }

  cout << maximum << endl;

  return 0;
}
