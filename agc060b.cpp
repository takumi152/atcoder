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

  int t;
  cin >> t;
  vector<int> n(t), m(t), k(t);
  vector<string> s(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i] >> m[i] >> k[i];
    cin >> s[i];
  }

  vector<string> ans(t);
  for (int i = 0; i < t; i++) {
    int turn_count = 0;
    bool blocked_vertical = true;
    bool blocked_horizontal = true;
    for (int j = 0; j < n[i] + m[i] - 2; j++) {
      if (j == 0 && s[i][j] == 'D') blocked_horizontal = false;
      if (j == 0 && s[i][j] == 'R') blocked_vertical = false;
      if (j > 0 && s[i][j-1] != s[i][j]) turn_count++;
      if (j > 0 && s[i][j-1] == 'D' && s[i][j] == 'D') blocked_horizontal = false;
      if (j > 0 && s[i][j-1] == 'R' && s[i][j] == 'R') blocked_vertical = false;
      if (j == n[i] + m[i] - 3 && s[i][j] == 'D') blocked_horizontal = false;
      if (j == n[i] + m[i] - 3 && s[i][j] == 'R') blocked_vertical = false;
    }
    int bit_required = turn_count;
    if (blocked_horizontal || blocked_vertical) bit_required--;
    if (k[i] >= bit_required) ans[i] = "Yes";
    else ans[i] = "No";
  }

  for (int i = 0; i < t; i++) {
    cout << ans[i] << endl;
  }

  return 0;
}
