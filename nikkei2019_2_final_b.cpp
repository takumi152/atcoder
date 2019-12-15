#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int n = s.length();

  ll ans = 0;
  for (int i = 1; i < n-4; i++) {
    for (int j = i+1; j < min(n-3, n-(j-i)); j++) {
      string subs = s.substr(i, j-i);
      if (subs == s.substr(n-(j-i), j-i)) {
        for (int k = 1; j + k * 2 < n-(j-i); k++) {
          if (s.substr(j, k) == s.substr(j+k, k)) ans++;
        }
      }
    }
  }

  cout << ans << endl;

  return 0;
}
