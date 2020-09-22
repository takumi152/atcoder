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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  ll ans = 0;
  ll req = a[0];
  for (int i = 1; i < n; i++) {
    if (req > a[i]) ans += req - a[i];
    else req = a[i];
  }

  cout << ans << endl;

  return 0;
}
