#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  int xor_sum = 0;
  for (auto &x: a) xor_sum ^= x;
  if (xor_sum != 0) {
    cout << -1 << endl;
    return 0;
  }

  unordered_map<int, int> item_count;
  for (auto &x: a) item_count[x]++;

  int ans = -1;
  for (auto &[k, v]: item_count) {
    if (v % 2 != 0) ans = max(ans, k - 1);
  }

  if (ans == -1) cout << 0 << endl;
  else cout << ans << endl;

  return 0;
}