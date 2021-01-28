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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m;
  cin >> n >> m;

  if (m == 1) {
    cout << 0 << endl;
    return 0;
  }

  vector<vector<int> > depth(m, vector<int>(m, -1));
  depth[0][1] = 0;
  ll div = 0;
  ll now = 1;
  ll op = 0;
  while (op < n) {
    now *= 10;
    div = (div * 10 + now / m) % m;
    now %= m;
    op++;
    // cerr << op << " " << div << " " << now << endl;
    if (depth[div][now] != -1) break;
    depth[div][now] = op;
  }
  // cerr << op - depth[div][now] << endl;

  if (op < n) {
    op += (depth[div][now] - op) * ((n - op) / (depth[div][now] - op));
    while (op < n) {
      now *= 10;
      div = (div * 10 + now / m) % m;
      now %= m;
      op++;
      // cerr << op << " " << div << " " << now << endl;
    }
  }

  cout << div << endl;

  return 0;
}
