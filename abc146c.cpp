#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

ll countDigit(ll x) {
  int a = 0;
  while (x > 0) {
    x /= 10;
    a++;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll a, b, x;
  cin >> a >> b >> x;

  ll left = 0;
  ll right = 1000000000;
  while (left < right) {
    ll center = (left + right + 1) / 2;
    ll p = center * a + countDigit(center) * b;
    if (p <= x) left = center;
    else right = center - 1;
  }

  cout << left << endl;

  return 0;
}
