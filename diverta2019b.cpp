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

  int r, g, b, n;
  cin >> r >> g >> b >> n;

  int ans = 0;
  for (int i = 0; i * r <= n; i++) {
    for (int j = 0; i * r + j * g <= n; j++) {
      if ((n - (i * r + j * g)) % b == 0) ans++;
    }
  }

  cout << ans << endl;

  return 0;
}
