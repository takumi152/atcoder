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

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  vector<bool> found(200002);
  int d = 0;
  vector<int> ans(n);
  for (int i = 0; i < n; i++) {
    found[p[i]] = true;
    while (found[d]) d++;
    ans[i] = d;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
