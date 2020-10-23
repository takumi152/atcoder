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
  vector<Pii> xy(n);
  for (auto &x: xy) cin >> x.first >> x.second;

  int ans = 0;
  for (int i = 0; i < n-1; i++) {
    ans += abs(xy[i].first - xy[i+1].first) + abs(xy[i].second - xy[i+1].second);
  }

  cout << ans << endl;

  return 0;
}
