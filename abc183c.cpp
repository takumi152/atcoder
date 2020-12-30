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

  int n, k;
  cin >> n >> k;
  vector<vector<int> > t(n, vector<int>(n));
  for (auto &x: t) {
    for (auto &y: x) cin >> y;
  }

  vector<int> v(n-1);
  for (int i = 0; i < n-1; i++) v[i] = i+1;

  int ans = 0;
  do {
    int total = 0;
    total += t[0][v[0]];
    for (int i = 1; i < n-1; i++) total += t[v[i-1]][v[i]];
    total += t[v[n-2]][0];
    if (total == k) ans++;
  } while (next_permutation(v.begin(), v.end()));

  cout << ans << endl;

  return 0;
}
