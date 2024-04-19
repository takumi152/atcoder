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

  int n, q;
  cin >> n >> q;
  string s;
  cin >> s;
  vector<Pii> lr(q);
  for (auto &[x, y]: lr) {
    cin >> x >> y;
    x--;
    y--;
  }

  vector<int> diff_sum(n);
  {
    int diff_count = 0;
    char prev_char = s[0];
    for (int i = 1; i < n; i++) {
      if (s[i] != prev_char) {
        diff_count++;
        prev_char = s[i];
      }
      diff_sum[i] = diff_count;
    }
  }

  vector<int> ans(q);
  for (int i = 0; i < q; i++) {
    auto [l, r] = lr[i];
    int diff = diff_sum[r] - diff_sum[l];
    ans[i] = (diff == 0) ? 0 : (diff + 1 + ((s[l] == s[r]) ? 0 : 1)) / 2;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
