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

  ll n, a, b;
  cin >> n >> a >> b;

  ll ab = a + b;
  ll loop = n / ab;
  ll remaining = n - ab * loop;

  ll bc = a * loop;
  ll ans = bc + min(remaining, a);

  cout << ans << endl;

  return 0;
}
