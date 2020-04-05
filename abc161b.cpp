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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  int total = 0;
  for (auto &x: a) total += x;

  int required = (total + 4 * m - 1) / (4 * m);
  int selectable = 0;
  for (auto &x: a) {
    if (x >= required) selectable++;
  }

  if (selectable >= m) {
    cout << "Yes" << endl;
  }
  else {
    cout << "No" << endl;
  }

  return 0;
}
