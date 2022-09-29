#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  sort(a.begin(), a.end());
  int maximum = a[0];

  unordered_set<int> creatable;
  for (auto &x: a) creatable.insert(x);
  int i = 0;
  while (i < n) {
    int p = a.size();
    for (int j = i+1; j < p; j++) {
      int g = gcd(a[i], a[j]);
      if (creatable.find(g) == creatable.end()) {
        creatable.insert(g);
        a.push_back(g);
      }
    }
    i++;
  }

  int ans = 0;
  for (auto &x: creatable) {
    if (x <= maximum) ans++;
  }

  cout << ans << endl;

  return 0;
}
