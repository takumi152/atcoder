#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Pii> ab(n);
  for (auto &[a, b]: ab) cin >> a >> b;

  vector<int> ans(n);
  for (int i = 0; i < n; i++) ans[i] = ab[i].first + ab[i].second;
  
  for (auto &x: ans) cout << x << endl;

  return 0;
}
