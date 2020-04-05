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

  int n, q;
  cin >> n >> q;
  vector<int> a(n), s(q);
  for (auto &x: a) cin >> x;
  for (auto &x: s) cin >> x;

  vector<int> gcdPartial(n);
  gcdPartial[0] = a[0];
  for (int i = 1; i < n; i++) gcdPartial[i] = gcd(gcdPartial[i-1], a[i]);

  vector<int> ans(q);
  for (int i = 0; i < q; i++) {
    int ng = 0;
    int ok = n;
    while (ng < ok) {
      int center = (ng + ok) / 2;
      if (gcd(gcdPartial[center], s[i]) == 1) ok = center;
      else ng = center + 1;
    }
    if (ng == n) ans[i] = gcd(gcdPartial[n-1], s[i]);
    else ans[i] = ok + 1;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
