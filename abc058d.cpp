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

  ll n, m;
  cin >> n >> m;
  vector<ll> x(n), y(m);
  for (auto &z: x) cin >> z;
  for (auto &z: y) cin >> z;

  vector<ll> multimex(n-1);
  for (int i = 0; i < n-1; i++) multimex[i] = (i + 1) * (n - i - 1);
  ll areax = 0;
  for (int i = 0; i < n-1; i++) {
    areax = (areax + ((x[i+1] - x[i]) * multimex[i]) % mod) % mod;
  }
  vector<ll> multimey(m-1);
  for (int i = 0; i < m-1; i++) multimey[i] = (i + 1) * (m - i - 1);
  ll areay = 0;
  for (int i = 0; i < m-1; i++) {
    areay = (areay + ((y[i+1] - y[i]) * multimey[i]) % mod) % mod;
  }

  ll ans = (areax * areay) % mod;

  cout << ans << endl;

  return 0;
}
