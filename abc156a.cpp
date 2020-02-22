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

  int n, r;
  cin >> n >> r;

  int ans = 0;
  if (n >= 10) ans = r;
  else ans = r + (100 * (10 - n));

  cout << ans << endl;

  return 0;
}
