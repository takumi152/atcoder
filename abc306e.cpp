#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k, q;
  cin >> n >> k >> q;
  vector<int> x(q), y(q);
  for (int i = 0; i < q; i++) {
    cin >> x[i] >> y[i];
    x[i]--;
  }

  vector<int> a(n);
  multiset<int> low, high;
  for (int i = 0; i < k; i++) high.insert(0);
  for (int i = k; i < n; i++) low.insert(0);

  int highest_low = 0;
  int lowest_high = 0;
  ll sum = 0;
  vector<ll> ans(q);
  for (int i = 0; i < q; i++) {
    int orig = a[x[i]];
    a[x[i]] = y[i];
    if (orig >= lowest_high) {
      high.erase(high.find(orig));
      sum -= orig;
      if (y[i] >= highest_low) {
        high.insert(y[i]);
        sum += y[i];
        lowest_high = *(high.begin());
      }
      else {
        low.insert(y[i]);
        low.erase(low.find(highest_low));
        high.insert(highest_low);
        sum += highest_low;
        lowest_high = highest_low;
        highest_low = *(low.rbegin());
      }
    }
    else {
      low.erase(low.find(orig));
      if (y[i] <= lowest_high) {
        low.insert(y[i]);
        highest_low = *(low.rbegin());
      }
      else {
        high.insert(y[i]);
        sum += y[i];
        high.erase(high.find(lowest_high));
        sum -= lowest_high;
        low.insert(lowest_high);
        highest_low = lowest_high;
        lowest_high = *(high.begin());
      }
    }
    ans[i] = sum;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
