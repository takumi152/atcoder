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

  int ans = 1000000007;
  for (int i = 1; i <= 100; i++) {
    int cost = 0;
    for (int j = 0; j < n; j++) {
      cost += (i - a[j]) * (i - a[j]);
    }
    if (cost < ans) {
      ans = cost;
    }
  }

  cout << ans << endl;

  return 0;
}
