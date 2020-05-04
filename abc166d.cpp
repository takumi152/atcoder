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

  ll x;
  cin >> x;

  for (ll i = -200; i <= 200; i++) {
    for (ll j = -200; j <= 200; j++) {
      if ((i*i*i*i*i) - (j*j*j*j*j) == x) {
        cout << i << " " << j << endl;
        return 0;
      }
    }
  }

  return 0;
}
