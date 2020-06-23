#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;
  int q;
  cin >> q;
  vector<vector<ll> > query(q, vector<ll>(3));
  for (int i = 0; i < q; i++) {
    cin >> query[i][0];
    if (query[i][0] != 3) cin >> query[i][1] >> query[i][2];
  }

  vector<ll> row(n), col(n);
  for (int i = 0; i < n; i++) {
    row[i] = i;
    col[i] = i;
  }
  vector<ll> ans;
  bool transposed = false;
  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      if (!transposed) swap(row[query[i][1]-1], row[query[i][2]-1]);
      else swap(col[query[i][1]-1], col[query[i][2]-1]);
    }
    else if (query[i][0] == 2) {
      if (!transposed) swap(col[query[i][1]-1], col[query[i][2]-1]);
      else swap(row[query[i][1]-1], row[query[i][2]-1]);
    }
    else if (query[i][0] == 3) {
      transposed = !transposed;
    }
    else {
      if (!transposed) ans.push_back(n * row[query[i][1]-1] + col[query[i][2]-1]);
      else ans.push_back(n * row[query[i][2]-1] + col[query[i][1]-1]);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
