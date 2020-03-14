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

  ll h, w;
  cin >> h >> w;

  ll ans = (h * w + 1) / 2;
  if (h == 1 || w == 1) ans = 1;

  cout << ans << endl;

  return 0;
}
