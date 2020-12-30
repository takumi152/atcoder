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

  int n, m;
  cin >> n >> m;

  if (n == 1 && m == 0) {
    cout << 1 << " " << 2 << endl;
    return 0;
  }

  if (m < 0 || m >= n - 1) {
    cout << -1 << endl;
    return 0;
  }

  vector<Pii> ans(n);
  int p = 0;

  ans[p] = Pii(1, 123456789);
  p++;

  for (int i = 0; i < m + 1; i++) {
    ans[p] = Pii(i * 2 + 2, i * 2 + 3);
    p++;
  }

  int w = 234567890;
  while (p < n) {
    ans[p] = Pii(w, w + 1);
    p++;
    w += 2;
  }

  for (auto &x: ans) cout << x.first << " " << x.second << endl;

  return 0;
}
