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
  vector<int> t(n);
  for (auto &x: t) cin >> x;

  int ans = 1000000007;
  for (int i = 0; i < (1 << n); i++) {
    int t1 = 0;
    int t2 = 0;
    for (int j = 0; j < n; j++) {
      if (i & (1 << j)) t1 += t[j];
      else t2 += t[j];
    }
    int tx = max(t1, t2);
    if (tx < ans) ans = tx;
  }

  cout << ans << endl;

  return 0;
}
