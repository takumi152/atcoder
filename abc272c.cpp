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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> odd, even;
  for (auto &x: a) {
    if (x % 2 == 0) even.push_back(x);
    else odd.push_back(x);
  }

  sort(odd.begin(), odd.end());
  sort(even.begin(), even.end());

  int ans = -1;
  if (odd.size() >= 2) ans = max(ans, odd[odd.size()-1] + odd[odd.size()-2]);
  if (even.size() >= 2) ans = max(ans, even[even.size()-1] + even[even.size()-2]);

  cout << ans << endl;

  return 0;
}
