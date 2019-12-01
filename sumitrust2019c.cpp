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

  int x;
  cin >> x;

  int ans = 0;
  if (x < 100) ans = 0;
  else if (x < 2000) {
    int q = x / 100;
    int r = x % 100;
    if (r > q * 5) ans = 0;
    else ans = 1;
  }
  else ans = 1;

  cout << ans << endl;

  return 0;
}
