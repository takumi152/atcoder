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

  ll n, k;
  cin >> n >> k;

  ll p = 1;
  int ans = 1;
  while (p < n) {
    p *= k;
    if (p > n) break;
    ans++;
  }

  cout << ans << endl;

  return 0;
}
