#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int k;
  cin >> k;

  ll ans = 0;
  for (int a = 1; a <= k; a++) {
    for (int b = 1; b <= k; b++) {
      for (int c = 1; c <= k; c++) {
        ans += gcd(gcd(a, b), c);
      }
    }
  }

  cout << ans << endl;

  return 0;
}
