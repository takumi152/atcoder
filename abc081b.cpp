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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  int ans = 1000000007;
  for (int i = 0; i < n; i++) {
    int div2 = 0;
    while (a[i] % 2 == 0) {
      div2++;
      a[i] /= 2;
    }
    if (div2 < ans) ans = div2;
  }

  cout << ans << endl;

  return 0;
}
