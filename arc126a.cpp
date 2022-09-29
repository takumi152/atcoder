#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<vector<ll> > n(t, vector<ll>(3));
  for (auto &x: n) {
    for (auto &y: x) cin >> y;
  }

  vector<vector<int> > pattern = {
                                  {5, 0, 0},
                                  {3, 0, 1},
                                  {2, 2, 0},
                                  {1, 0, 2},
                                  {0, 2, 1}
                                 };

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    vector<int> order = {0, 1, 2, 3, 4};
    do {
      ll n2 = n[i][0];
      ll n3 = n[i][1];
      ll n4 = n[i][2];
      ll score = 0;
      for (int j = 0; j < 5; j++) {
        ll maximum = (ll) 9e18;
        if (pattern[order[j]][0] != 0) maximum = min(maximum, n2 / pattern[order[j]][0]);
        if (pattern[order[j]][1] != 0) maximum = min(maximum, n3 / pattern[order[j]][1]);
        if (pattern[order[j]][2] != 0) maximum = min(maximum, n4 / pattern[order[j]][2]);
        score += maximum;
        n2 -= maximum * pattern[order[j]][0];
        n3 -= maximum * pattern[order[j]][1];
        n4 -= maximum * pattern[order[j]][2];
      }
      if (score > ans[i]) ans[i] = score;
    } while (next_permutation(order.begin(), order.end()));
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
