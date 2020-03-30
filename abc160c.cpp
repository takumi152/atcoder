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

  int k, n;
  cin >> k >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  a.push_back(a[0] + k);
  int ans = 1000000007;
  for (int i = 0; i < n; i++) {
    int d = k - (a[i+1] - a[i]);
    if (d < ans) ans = d;
  }

  cout << ans << endl;

  return 0;
}
