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

  int n, x, t;
  cin >> n >> x >> t;

  int ans = 0;
  for (int i = 0; i < n; i += x) ans += t;

  cout << ans << endl;

  return 0;
}
