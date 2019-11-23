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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int m;
  cin >> m;
  vector<ll> d(m), c(m);
  for (int i = 0; i < m; i++) cin >> d[i] >> c[i];

  ll ans = -1;
  for (int i = 0; i < m; i++) ans += c[i];

  ll s = 0;
  for (int i = 0; i < m; i++) s += d[i] * c[i];
  ans += (s - 1) / 9;

  cout << ans << endl;

  return 0;
}
