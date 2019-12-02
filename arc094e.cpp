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
  vector<ll> a(n), b(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

  bool allSame = true;
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      allSame = false;
      break;
    }
  }

  if (allSame) {
    cout << 0 << endl; // no moves
    return 0;
  }

  ll ans = 0;
  ll minLargeCount = 1000000007;
  for (int i = 0; i < n; i++) {
    ans += a[i];
    if (a[i] > b[i] && b[i] < minLargeCount) {
      minLargeCount = b[i];
    }
  }
  ans -= minLargeCount;

  cout << ans << endl;

  return 0;
}
