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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, a, b;
  cin >> n >> a >> b;
  vector<int> c(n);
  for (auto &x: c) cin >> x;

  int ans = a + b;
  for (int i = 0; i < n; i++) {
    if (c[i] == ans) {
      cout << i + 1 << endl;
      break;
    }
  }

  return 0;
}
