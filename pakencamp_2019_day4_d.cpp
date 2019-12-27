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

  int n, x;
  cin >> n >> x;

  int k = 1001;
  vector<int> ans(n, 869120);
  int c = 0;
  for (int i = 0; i < n; i++) {
    if (c == x) break;
    else if (c + i + 1 <= x) {
      ans[i] = 1;
      c += i + 1;
    }
    else {
      ans[i] = k - (x - c) + 1;
      c = x;
    }
  }

  if (c < x) {
    cout << -1 << endl; // impossible
    return 0;
  }
  else {
    cout << k << endl;
    for (auto &y: ans) cout << y << " ";
    return 0;
  }

  return 0;
}
