#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;
  int k;
  cin >> k;

  int m = s.size();
  vector<int> n(m);
  for (int i = 0; i < m; i++) n[i] = s[i] - '0';

  ll ans = (m - 1) * 9;
  if (k >= 2) ans = (ans * (m - 2) * 9) / 2;
  if (k >= 3) ans = (ans * (m - 3) * 9) / 3;

  while (k > 0) {
    ans *= n[0];
    n.erase(n.begin());
    m--;
    while (m > 0) {
      if (n[0] == 0) {
        n.erase(n.begin());
        m--;
      }
      else break;
    }
    k--;
  }
  
  cout << ans << endl;

  return 0;
}
