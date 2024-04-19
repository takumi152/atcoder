#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m;
  cin >> n >> m;

  int largest_digit = 0;
  int largest_num = 0;

  for (int i = 1; i <= 9; i++) {
    ll res = 0;
    for (int d = 1; d <= n; d++) {
      res = (res * 10 + i) % m;
      if (res == 0) {
        if (d > largest_digit || (d == largest_digit && i > largest_num)) {
          largest_digit = d;
          largest_num = i;
        }
      }
    }
  }

  if (largest_digit > 0) {
    for (int d = 0; d < largest_digit; d++) cout << largest_num;
    cout << endl;
  }
  else {
    cout << -1 << endl;
  }
}
