#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <cctype>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

struct query {
  int t, x;
  char c;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string s;
  cin >> s;
  int q;
  cin >> q;
  vector<query> queries(q);
  for (auto &x: queries) cin >> x.t >> x.x >> x.c;

  string ans = s;
  vector<int> last_changed(n, -1);
  int last_lower = -1;
  int last_upper = -1;
  for (int i = 0; i < q; i++) {
    int t = queries[i].t;
    if (t == 1) {
      int x = queries[i].x;
      char c = queries[i].c;
      ans[x-1] = c;
      last_changed[x-1] = i;
    }
    else if (t == 2) {
      last_lower = i;
    }
    else if (t == 3) {
      last_upper = i;
    }
  }

  for (int i = 0; i < n; i++) {
    if (last_lower == -1 && last_upper == -1) continue;
    else if (last_lower > last_upper && last_changed[i] < last_lower) ans[i] = tolower(ans[i]);
    else if (last_lower < last_upper && last_changed[i] < last_upper) ans[i] = toupper(ans[i]);
  }

  cout << ans << endl;

  return 0;
}
