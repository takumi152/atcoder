#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<pair<ll, ll> > xd(n);
  for (auto &x: xd) cin >> x.first >> x.second;

  sort(xd.begin(), xd.end());
  list<pair<ll, ll> > group;
  group.push_front(pair<ll, ll>(2, xd[n-1].first));
  for (int i = n-2; i >= 0; i--) {
    if (xd[i].first + xd[i].second - 1 < group.begin()->second) {
      group.push_front(pair<ll, ll>(2, xd[i].first));
    }
    else {
      while(true) {
        auto it1 = group.begin();
        auto it2 = group.begin();
        it2++;
        if (it2 == group.end()) break;
        if (xd[i].first + xd[i].second - 1 >= it2->second) {
          it2->first = (it1->first * it2->first) % mod;
          group.erase(it1);
        }
        else break;
      }
      group.begin()->first = (group.begin()->first + 1) % mod;
      group.begin()->second = xd[i].first;
    }
  }

  ll ans = 1;
  for (auto &x: group) {
    ans = (ans * x.first) % mod;
  }

  cout << ans << endl;

  return 0;
}
