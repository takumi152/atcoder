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

  ll ans = 0;
  ans += (x / 500) * 1000;
  x %= 500;
  ans += (x / 5) * 5;

  cout << ans << endl;

  return 0;
}
