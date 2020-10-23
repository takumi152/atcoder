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

  int t;
  cin >> t;
  vector<int> n(t);
  vector<vector<ll> > a(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i];
    a[i] = vector<ll>(n[i]);
    for (auto &x: a[i]) cin >> x;
  }

  vector<string> ans(t);
  for (int i = 0; i < t; i++) {
    if (n[i] % 2 == 0) {
      // f -> xor != 0
      // s -> xor == 0
      // 先手は大きいやつから取ればよい
      sort(a[i].rbegin(), a[i].rend());
      ll first = 0;
      ll second = 0;
      for (int j = 0; j < n[i]; j++) {
        if (j % 2 == 0) first += a[i][j];
        else second += a[i][j];
      }
      if (first > second) ans[i] = "First";
      else ans[i] = "Second";
    }
    else {
      // 後手必勝？
      ans[i] = "Second";
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
