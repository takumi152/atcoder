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

  int n, k, m;
  cin >> n >> k >> m;
  vector<int> a(n-1);
  for (auto &x: a) cin >> x;

  int total = 0;
  for (auto &x: a) total += x;

  for (int i = 0; i <= k; i++) {
    if ((total + i) / n >= m) {
      cout << i << endl;
      return 0;
    }
  }

  cout << -1 << endl;

  return 0;
}
