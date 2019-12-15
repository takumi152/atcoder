#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  vector<ll> count(5);
  for (int i = 0; i < n; i++) {
    if (s[i][0] == 'M') count[0]++;
    if (s[i][0] == 'A') count[1]++;
    if (s[i][0] == 'R') count[2]++;
    if (s[i][0] == 'C') count[3]++;
    if (s[i][0] == 'H') count[4]++;
  }

  ll ans = 0;
  ans += count[0] * count[1] * count[2];
  ans += count[0] * count[1] * count[3];
  ans += count[0] * count[1] * count[4];
  ans += count[0] * count[2] * count[3];
  ans += count[0] * count[2] * count[4];
  ans += count[0] * count[3] * count[4];
  ans += count[1] * count[2] * count[3];
  ans += count[1] * count[2] * count[4];
  ans += count[1] * count[3] * count[4];
  ans += count[2] * count[3] * count[4];

  cout << ans << endl;

  return 0;
}
