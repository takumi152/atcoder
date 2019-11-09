#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;

  if (n < (k - 1) * 2 + 1) {
    cout << -1 << endl; // impossible
    return 0;
  }

  vector<vector<int> > ans(n, vector<int>(3));
  int ap, bp, cp, pp;
  ap = n/2;
  bp = 0;
  cp = 0;
  pp = 0;
  while (ap < n) {
    ans[pp][0] = k + ap;
    ans[pp][1] = k + bp + n;
    ans[pp][2] = k + cp + n * 2;
    ap++;
    bp++;
    cp += 2;
    pp++;
  }
  ap = 0;
  cp = 1;
  while (cp < n) {
    ans[pp][0] = k + ap;
    ans[pp][1] = k + bp + n;
    ans[pp][2] = k + cp + n * 2;
    ap++;
    bp++;
    cp += 2;
    pp++;
  }

  for (auto &x: ans) {
    cout << x[0] << " " << x[1] << " " << x[2] << endl;
  }

  return 0;
}
