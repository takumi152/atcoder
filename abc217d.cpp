#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int l, q;
  cin >> l >> q;
  vector<Pii> query(q);
  for (auto &x: query) cin >> x.first >> x.second;

  set<int> wood;
  wood.insert(0);
  wood.insert(l);

  vector<int> ans;
  for (int i = 0; i < q; i++) {
    if (query[i].first == 1) {
      wood.insert(query[i].second);
    }
    else {
      auto it1 = wood.lower_bound(query[i].second);
      auto it2 = it1;
      it1--;
      ans.push_back(*it2 - *it1);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
