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

  int ans = -1;
  for (int i = 0; i <= n; i++) {
    int p = i * 108 / 100;
    if (p == n) ans = i;
  }

  if (ans == -1) cout << ":(" << endl;
  else cout << ans << endl;

  return 0;
}
