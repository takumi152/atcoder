#include <iostream>
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

  int n, m;
  cin >> n >> m;
  vector<int> s(m), c(m);
  for (int i = 0; i < m; i++) cin >> s[i] >> c[i];

  vector<int> num(n, -1);
  for (int i = 0; i < m; i++) {
    if (num[n-s[i]] == -1) {
      num[n-s[i]] = c[i];
    }
    else if (num[n-s[i]] != c[i]) {
      cout << -1 << endl;
      return 0;
    }
  }
  if (n >= 2 && num[n-1] == 0) {
    cout << -1 << endl;
    return 0;
  }
  if (n >= 2 && num[n-1] == -1) {
    num[n-1] = 1;
  }
  if (n == 1 && num[n-1] == -1) {
    num[n-1] = 0;
  }
  for (int i = n-2; i >= 0; i--) {
    if (num[i] == -1) num[i] = 0;
  }

  int ans = 0;
  int mul = 1;
  for (int i = 1; i < n; i++) mul *= 10;
  for (int i = n-1; i >= 0; i--) {
    ans += mul * num[i];
    mul /= 10;
  }

  cout << ans << endl;

  return 0;
}
