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

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int a, b;
  cin >> a >> b;

  int l = max(0, a - b * 2);

  cout << l << endl;

  return 0;
}
