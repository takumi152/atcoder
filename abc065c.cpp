#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll N, M;
  cin >> N >> M;

  ll a = max(N, M);
  ll b = min(N, M);
  ll ans = 1;

  if (a - b >= 2) {
    cout << 0 << endl; // impossible
    return 0;
  }
  else if (a - b == 1) {
    bool putFromA = true;
    while (a > 0) {
      if (putFromA) {
        ans = (ans * a) % mod;
        a--;
      }
      else {
        ans = (ans * b) % mod;
        b--;
      }
      putFromA = !putFromA;
    }
  }
  else {
    bool putFromA = true;
    while (b > 0) {
      if (putFromA) {
        ans = (ans * a) % mod;
        a--;
      }
      else {
        ans = (ans * b) % mod;
        b--;
      }
      putFromA = !putFromA;
    }
    ans = (ans * 2) % mod;
  }

  cout << ans << endl;

  return 0;
}
