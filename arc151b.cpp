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

  int n;
  cin >> n;
  vector<char> s(n), t(n);
  for (auto &x: s) cin >> x;
  for (auto &x: t) cin >> x;

  int hamming_distance = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] != t[i]) hamming_distance++;
  }
  if (hamming_distance % 2 != 0) {
    cout << -1 << endl;
    return 0;
  }

  int s_zero = 0;
  int t_zero = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] != t[i]) {
      if (s[i] == '0') s_zero++;
      else t_zero++;
    }
  }

  vector<char> ans(n, '0');
  for (int i = n-1; i >= 0; i--) {
    if (s[i] != t[i]) {
      if (s_zero > t_zero && s[i] == '0') {
        ans[i] = '1';
        s_zero -= 2;
      }
      else if (s_zero < t_zero && t[i] == '0') {
        ans[i] = '1';
        t_zero -= 2;
      }
    }
  }

  for (auto &x: ans) cout << x;
  cout << endl;

  return 0;
}
