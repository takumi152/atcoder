#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;

  vector<int> x;
  ll t = n;
  while (t > 0) {
    x.push_back(t % 10);
    t /= 10;
  }
  x.push_back(0);

  ll ans = 0;
  int d = x.size() - 1;
  ll k = 10;
  for (int i = 0; i < d; i++) {
    if (x[i] == 0) ans += n / k * (k / 10);
    else if (x[i] == 1) ans += n / k * (k / 10) + (n % (k / 10)) + 1;
    else ans += (n + k - 1) / k * (k / 10);
    k *= 10;
  }

  cout << ans << endl;

  return 0;
}
