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

  ll n, w;
  cin >> n >> w;
  vector<ll> s(n), t(n), p(n);
  for (int i = 0; i < n; i++) cin >> s[i] >> t[i] >> p[i];

  vector<ll> imos(200001);
  for (int i = 0; i < n; i++) {
    imos[s[i]] += p[i];
    imos[t[i]] -= p[i];
  }

  ll usage = 0;
  for (int i = 0; i <= 200000; i++) {
    usage += imos[i];
    if (usage > w) {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
