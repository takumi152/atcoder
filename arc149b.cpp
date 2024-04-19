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
  for (int i = 0; i < n; i++) cin >> ab[i].first;
  for (int i = 0; i < n; i++) cin >> ab[i].second;

  sort(ab.begin(), ab.end());

  vector<int> dp;
  for (int i = 0; i < n; i++) {
    auto it = lower_bound(dp.begin(), dp.end(), ab[i].second);
    if (it == dp.end()) dp.push_back(ab[i].second);
    else *it = ab[i].second;
  }

  cout << n + dp.size() << endl;
}
