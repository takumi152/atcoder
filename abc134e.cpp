#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> a(N);
  for (auto &x: a) cin >> x;

  for (auto &x: a) x = -x; // invert sign

  multiset<int> maxCol;
  maxCol.insert(a[0]);
  for (int i = 1; i < N; i++) {
    auto it = maxCol.upper_bound(a[i]);
    if (it != maxCol.end()) {
      maxCol.erase(it);
    }
    maxCol.insert(a[i]);
  }

  cout << maxCol.size() << endl;

  return 0;
}
