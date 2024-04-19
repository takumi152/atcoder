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

  ll l, r;
  cin >> l >> r;

  vector<pair<ll, ll>> ans;
  ll num = l;
  while (num < r) {
    ll first_set = __builtin_ffsll(num) - 1;
    ll val_to_add;
    if (first_set == -1) {
      val_to_add = 1LL << (64LL - (ll) __builtin_clzll(r));
    }
    else {
      val_to_add = 1LL << first_set;
    }
    while (num + val_to_add > r) {
      val_to_add >>= 1;
    }
    ans.emplace_back(num, num + val_to_add);
    num += val_to_add;
  }

  cout << ans.size() << endl;
  for (auto &p: ans) {
    cout << p.first << " " << p.second << endl;
  }

  return 0;
}
