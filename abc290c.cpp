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

  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  sort(a.begin(), a.end());
  auto unique_end = unique(a.begin(), a.end());
  int unique_count = distance(a.begin(), unique_end);

  for (int i = 0; i < unique_count; i++) {
    if (a[i] != i || k <= i) {
      cout << i << endl;
      return 0;
    }
  }

  cout << unique_count << endl;

  return 0;
}
