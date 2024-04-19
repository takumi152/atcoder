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

  int n, m;
  cin >> n >> m;
  string s, t;
  cin >> s >> t;

  int ans = 0;
  if (s != t.substr(0, n)) {
    ans |= 2;
  }
  if (s != t.substr(m - n, n)) {
    ans |= 1;
  }

  cout << ans << endl;

  return 0;
}
