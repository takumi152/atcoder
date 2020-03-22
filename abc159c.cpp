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

  int l;
  cin >> l;

  double len = (double) l / 3.0;

  double ans = len * len * len;

  cout << setprecision(16) << ans << endl;

  return 0;
}
