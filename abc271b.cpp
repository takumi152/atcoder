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

  int n, q;
  cin >> n >> q;
  vector<vector<int>> a(n);
  for (int i = 0; i < n; i++) {
    int l;
    cin >> l;
    a[i] = vector<int>(l);
    for (int j = 0; j < l; j++) cin >> a[i][j];
  }
  vector<int> s(q), t(q);
  for (int i = 0; i < q; i++) cin >> s[i] >> t[i];

  for (int i = 0; i < q; i++) cout << a[s[i]-1][t[i]-1] << endl;

  return 0;
}
