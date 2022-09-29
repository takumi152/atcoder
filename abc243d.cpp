#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, x;
  cin >> n >> x;
  string s;
  cin >> s;

  string movement_from_root = "";
  while (x > 1) {
    if (x % 2 == 0) movement_from_root += "L";
    else movement_from_root += "R";
    x /= 2;
  }

  reverse(movement_from_root.begin(), movement_from_root.end());
  for (auto &c: s) {
    if (c == 'U') movement_from_root.pop_back();
    else movement_from_root.push_back(c);
  }

  ll ans = 1;
  for (auto &c: movement_from_root) {
    if (c == 'L') ans = ans * 2;
    else ans = ans * 2 + 1;
  }

  cout << ans << endl;

  return 0;
}
