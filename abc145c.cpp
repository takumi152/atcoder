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

  int n;
  cin >> n;
  vector<vector<int> > xy(n, vector<int>(2));
  for (auto &x: xy) cin >> x[0] >> x[1];

  vector<int> perm(n);
  for (int i = 0; i < n; i++) perm[i] = i;
  double total = 0.0;
  int count = 0;
  do {
    for (int i = 0; i < n-1; i++) {
      total += sqrt((xy[perm[i+1]][0] - xy[perm[i]][0]) * (xy[perm[i+1]][0] - xy[perm[i]][0]) + (xy[perm[i+1]][1] - xy[perm[i]][1]) * (xy[perm[i+1]][1] - xy[perm[i]][1]));
    }
    count++;
  } while (next_permutation(perm.begin(), perm.end()));

  double ans = total / count;

  cout << setprecision(16) << ans << endl;

  return 0;
}
