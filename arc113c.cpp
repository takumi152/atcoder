#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int n = s.size();

  set<vector<int> > section;
  for (int i = 0; i < n; i++) {
    section.insert(vector<int>({i, i+1, (int) s[i]}));
  }

  ll ans = 0;
  for (int i = n-1; i >= 2; i--) {
    if (s[i-2] == s[i-1] && s[i-1] != s[i]) {
      auto it = section.lower_bound(vector<int>({i}));
      while (it != section.end()) {
        if ((*it)[2] == (int) s[i-2]) ans -= (*it)[1] - (*it)[0];
        it = section.erase(it);
      }
      section.insert(vector<int>({i, n, (int) s[i-2]}));
      ans += n - i;
    }
  }

  cout << ans << endl;

  return 0;
}
