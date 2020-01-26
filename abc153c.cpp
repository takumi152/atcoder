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

  int n, k;
  cin >> n >> k;
  vector<ll> h(n);
  for (auto &x: h) cin >> x;

  sort(h.rbegin(), h.rend());

  ll ans = 0;
  for (int i = k; i < n; i++) ans += h[i];

  cout << ans << endl;

  return 0;
}
