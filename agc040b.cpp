// WIP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <queue>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<pair<int, int> > lr(n);
  for (auto &x: lr) cin >> x.first >> x.second;

  sort(lr.begin(), lr.end(), [](auto &a, auto &b){return (a.second - a.first) < (b.second - b.first);});

  int l1 = lr[n-1].first;
  int r1 = lr[n-1].second;

  return 0;
}
