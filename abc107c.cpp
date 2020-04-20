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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  int ans = 1000000007;
  for (int i = 0; i < n-k+1; i++) {
    int d = max(0, -a[i]) + max(0, a[i+k-1]) + min(max(0, -a[i]), max(0, a[i+k-1]));
    if (d < ans) ans = d;
  }

  cout << ans << endl;

  return 0;
}
