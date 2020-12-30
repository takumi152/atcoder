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

  ll n;
  cin >> n;

  ll ok = 0;
  ll ng = 4000000000;
  while (abs(ok - ng) > 1) {
    ll mid = (ok + ng) / 2;
    if (mid * (mid + 1) / 2 <= n + 1) {
      ok = mid;
    }
    else {
      ng = mid;
    }
  }

  cout << n - ok + 1 << endl;

  return 0;
}
