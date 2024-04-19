#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int m;
  cin >> m;
  vector<int> d(m);
  for (auto &x: d) cin >> x;

  int day_per_year = accumulate(d.begin(), d.end(), 0);

  int target_day = day_per_year / 2;
  int days_passed = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < d[i]; j++) {
      if (days_passed == target_day) {
        cout << i + 1 << " " << j + 1 << endl;
        return 0;
      }
      days_passed++;
    }
  }

  return 0;
}
