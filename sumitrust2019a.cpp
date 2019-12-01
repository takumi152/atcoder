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

  int m1, d1, m2, d2;
  cin >> m1 >> d1;
  cin >> m2 >> d2;

  int ans = 0;
  if (m1 < m2) ans = 1;

  cout << ans << endl;

  return 0;
}
