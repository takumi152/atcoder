#include <iostream>
#include <iomanip>
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

  int n;
  cin >> n;
  string s;
  cin >> s;
  string t;
  cin >> t;

  ll ans = 0;
  queue<int> up, down;
  for (int i = 0; i < n; i++) {
    if (s[i] == '1') up.push(i);
    if (t[i] == '1') down.push(i);
    if (up.size() == 2) {
      int p1 = up.front();
      up.pop();
      int p2 = up.front();
      up.pop();
      ans += p2 - p1;
    }
    if (up.size() >= 1 && down.size() >= 1) {
      if (up.front() >= down.front()) {
        int p1 = down.front();
        down.pop();
        int p2 = up.front();
        up.pop();
        ans += p2 - p1;
      }
    }
  }

  if (!up.empty() || !down.empty()) cout << -1 << endl;
  else cout << ans << endl;

  return 0;
}
