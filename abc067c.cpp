#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

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

  ll total = 0;
  for (auto &x: a) total += x;

  ll alpha = a[0];
  ll beta = total - a[0];
  ll minimum = abs(alpha - beta);
  for (int i = 1; i < n-1; i++) {
    alpha += a[i];
    beta -= a[i];
    if (abs(alpha - beta) < minimum) minimum = abs(alpha - beta);
  }

  cout << minimum << endl;

  return 0;
}
