#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  vector<int> q(n);
  for (int i = 0; i < n; i++) {
    q[p[i] - 1] = i + 1;
  }

  for (auto &x: q) cout << x << " ";
  cout << endl;

  return 0;
}
