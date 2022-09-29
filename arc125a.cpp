#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<int> s(n);
  for (auto &x: s) cin >> x;
  vector<int> t(m);
  for (auto &x: t) cin >> x;

  int ans = 0;
  bool first_rotate = true;
  int prev = s[0];
  for (int i = 0; i < m; i++) {
    if (prev != t[i]) {
      if (first_rotate) {
        int left_distance = 1000000007;
        int right_distance = 1000000007;
        for (int j = n-1; j > 0; j--) {
          if (s[j] == t[i]) {
            left_distance = (n-1) - j + 1;
            break;
          }
        }
        for (int j = 1; j < n; j++) {
          if (s[j] == t[i]) {
            right_distance = j;
            break;
          }
        }
        if (left_distance == 1000000007 && right_distance == 1000000007) {
          cout << -1 << endl; // impossible
          return 0;
        }
        else {
          ans += min(left_distance, right_distance) + 1;
          first_rotate = false;
        }
      }
      else ans += 2;
      prev = t[i];
    }
    else ans++;
  }

  cout << ans << endl;

  return 0;
}
