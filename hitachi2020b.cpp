#include <iostream>
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

  int a, b, m;
  cin >> a >> b >> m;
  vector<int> alpha(a), beta(b);
  for (auto &x: alpha) cin >> x;
  for (auto &x: beta) cin >> x;
  vector<vector<int> > xyc(m, vector<int>(3));
  for (int i = 0; i < m; i++) cin >> xyc[i][0] >> xyc[i][1] >> xyc[i][2];

  int minimum = 1000000007;
  for (int i = 0; i < m; i++) {
    int discountPrice = alpha[xyc[i][0]-1] + beta[xyc[i][1]-1] - xyc[i][2];
    if (discountPrice < minimum) minimum = discountPrice;
  }

  sort(alpha.begin(), alpha.end());
  sort(beta.begin(), beta.end());
  if (alpha[0] + beta[0] < minimum) minimum = alpha[0] + beta[0];

  cout << minimum << endl;

  return 0;
}
