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
  vector<vector<ll> > a(t, vector<ll>(6));
  for (int i = 0; i < t; i++) {
    for (auto &x: a[i]) cin >> x;
  }

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    vector<ll> x = {a[i][0], a[i][2], a[i][4]};
    vector<ll> y = {a[i][1], a[i][3], a[i][5]};
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());
    ll x_pivot = x[0];
    ll y_pivot = y[0];
    int phase = (x[0] == x[1]) + ((y[0] == y[1]) << 1);
    ans[i] = max(abs(x_pivot), abs(y_pivot)) * 2;
    if (x_pivot == 0 && y_pivot == 0) {
      if (phase != 3) ans[i]++;
    }
    else if (x_pivot == 0 && y_pivot > 0) {
      if (!(phase & 2)) ans[i]++;
    }
    else if (x_pivot > 0 && y_pivot > 0) {
      if (abs(x_pivot) == abs(y_pivot)) {
        if (phase == 0) ans[i] += 2;
        else ans[i]++;
      }
      else if (abs(x_pivot) > abs(y_pivot) && !(phase & 1)) ans[i]++;
      else if (abs(x_pivot) < abs(y_pivot) && !(phase & 2)) ans[i]++;
    }
    else if (x_pivot > 0 && y_pivot == 0) {
      if (!(phase & 1)) ans[i]++;
    }
    else if (x_pivot > 0 && y_pivot < 0) {
      if (abs(x_pivot) == abs(y_pivot) && phase == 2) ans[i]++;
      else if (abs(x_pivot) > abs(y_pivot) && !(phase & 1)) ans[i]++;
      else if (abs(x_pivot) < abs(y_pivot) && !(phase & 2)) ans[i]--;
    }
    else if (x_pivot == 0 && y_pivot < 0) {
      if (!(phase & 2)) ans[i]--;
    }
    else if (x_pivot < 0 && y_pivot < 0) {
      if (abs(x_pivot) == abs(y_pivot) && phase == 3) ans[i]++;
      else if (abs(x_pivot) > abs(y_pivot) && !(phase & 1)) ans[i]--;
      else if (abs(x_pivot) < abs(y_pivot) && !(phase & 2)) ans[i]--;
    }
    else if (x_pivot < 0 && y_pivot == 0) {
      if (!(phase & 1)) ans[i]--;
    }
    else if (x_pivot < 0 && y_pivot > 0) {
      if (abs(x_pivot) == abs(y_pivot) && phase == 1) ans[i]++;
      else if (abs(x_pivot) > abs(y_pivot) && !(phase & 1)) ans[i]--;
      else if (abs(x_pivot) < abs(y_pivot) && !(phase & 2)) ans[i]++;
    }
    cerr << x_pivot << " " << y_pivot << " " << phase << " " << ans[i] << endl;
  }

  // for (auto &x: ans) cout << x << endl;

  return 0;
}
