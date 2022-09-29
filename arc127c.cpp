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

  int n;
  cin >> n;
  string s;
  cin >> s;

  reverse(s.begin(), s.end());

  vector<bool> m(n);
  for (int i = 0; i < (int) s.size(); i++) {
    if (s[i] == '1') m[i] = true;
  }

  auto decrement = [](vector<bool> &x) {
    for (int i = 0; i < (int) x.size(); i++) {
      if (x[i]) {
        x[i] = false;
        return;
      }
      else {
        x[i] = true;
      }
    }
  };

  auto check_all_zero = [](vector<bool> &x) {
    for (int i = 0; i < (int) x.size(); i++) {
      if (x[i]) return false;
    }
    return true;
  };

  string ans = "1";
  {
    int p = n-1;
    while (p > 0) {
      decrement(m);
      while (p > 0) {
        if (!m[p]) break;
        else {
          m[p] = false;
          ans.push_back('1');
          p--;
        }
      }
      if (p == 0) break;
      if (check_all_zero(m)) break;
      p--;
      ans.push_back('0');
      // cerr << p << " ";
      // for (int j = n-1; j >= 0; j--) {
      //   if (m[j]) cerr << 1;
      //   else cerr << 0;
      // }
      // cerr << " " << ans << endl;
    }
  }

  cout << ans << endl;

  return 0;
}
