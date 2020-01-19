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
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  int minimum = p[0];
  int ans = 1;
  for (int i = 1; i < n; i++) {
    if (minimum >= p[i]) {
      ans++;
      minimum = p[i];
    }
  }

  cout << ans << endl;

  return 0;
}
