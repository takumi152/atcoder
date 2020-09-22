#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w, m;
  cin >> h >> w >> m;
  vector<Pii> t(m);
  for (auto &x: t) cin >> x.first >> x.second;

  set<Pii> t_set;
  for (auto &x: t) t_set.insert(x);

  unordered_map<int, int> rowcount, colcount;
  for (auto &x: t) {
    rowcount[x.first]++;
    colcount[x.second]++;
  }

  vector<int> rowmax, colmax;
  int rcmax = 0, ccmax = 0;
  for (auto &x: rowcount) {
    if (x.second > rcmax) {
      rcmax = x.second;
      rowmax.clear();
    }
    if (x.second == rcmax) {
      rowmax.push_back(x.first);
    }
  }
  for (auto &x: colcount) {
    if (x.second > ccmax) {
      ccmax = x.second;
      colmax.clear();
    }
    if (x.second == ccmax) {
      colmax.push_back(x.first);
    }
  }

  bool nodup = false;
  for (auto &x: rowmax) {
    for (auto &y: colmax) {
      if (t_set.find(Pii(x, y)) == t_set.end()) {
        nodup = true;
        break;
      }
    }
    if (nodup) break;
  }

  int ans = rcmax + ccmax;
  if (!nodup) ans--;

  cout << ans << endl;

  return 0;
}
