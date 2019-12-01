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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  ll ans = 1;
  vector<int> c(3);
  for (int i = 0; i < n; i++) {
    ll match = 0;
    for (int j = 0; j < 3; j++) {
      if (a[i] == c[j]) match++;
    }
    ans = (ans * match) % mod;
    for (int j = 0; j < 3; j++) {
      if (a[i] == c[j]) {
        c[j]++;
        break;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
