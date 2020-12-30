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
  vector<int> a(m);
  for (auto &x: a) cin >> x;

  a.push_back(0);
  a.push_back(n+1);
  sort(a.begin(), a.end());

  int k = n;
  for (int i = 0; i < m+1; i++) {
    if (a[i+1] - a[i] > 1) k = min(k, a[i+1] - a[i] - 1);
  }

  int ans = 0;
  for (int i = 0; i < m+1; i++) {
    if (a[i+1] - a[i] > 1) ans += (a[i+1] - a[i] - 1 + (k - 1)) / k;
  }

  cout << ans << endl;

  return 0;
}
