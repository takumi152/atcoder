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

  int n, m;
  cin >> n >> m;
  vector<ll> a(n), b(m);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  sort(a.rbegin(), a.rend());
  sort(b.rbegin(), b.rend());

  a.push_back(0); // sentinel
  b.push_back(0); // sentinel

  ll ans = 1;
  ll vacancy = 0;
  int pa = 0;
  int pb = 0;
  for (ll i = n * m; i > 0; i--) {
    if (a[pa] == i && b[pb] == i) {
      pa++;
      pb++;
      vacancy += pa + pb - 1;
      //ans = ans * 1;
      vacancy--;
    }
    else if (a[pa] == i) {
      pa++;
      vacancy += pb;
      ans = (ans * pb) % mod;
      vacancy--;
    }
    else if (b[pb] == i) {
      pb++;
      vacancy += pa;
      ans = (ans * pa) % mod;
      vacancy--;
    }
    else {
      ans = (ans * vacancy) % mod;
      vacancy--;
    }
    if (vacancy < 0) break;
  }

  cout << ans << endl;

  return 0;
}
