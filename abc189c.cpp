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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  int ans = 0;
  for (int i = 0; i < n; i++) {
    int minimum = a[i];
    for (int j = i; j < n; j++) {
      minimum = min(minimum, a[j]);
      ans = max(ans, minimum * (j - i + 1));
    }
  }

  cout << ans << endl;

  return 0;
}
