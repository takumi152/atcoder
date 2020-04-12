// WIP

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <numeric>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, k;
  cin >> n >> k;

  vector<ll> phi(k+1);
  for (ll i = 1; i <= k; i++) phi[i] = i;
  for (ll i = 2; i <= k; i++) {
    if (phi[i] == i) {
      phi[i] = i-1;
      for (ll j = (i << 1); j <= k; j+=i) {
        phi[j] = (phi[j]/i)*(i-1);
      }
    }
  }

  /*
  for (int i = 1; i < k; i++) {
    vector<int> res(i);
    for (int j = 1; j < i; j++) {
      res[gcd(i, j)-1]++;
    }
    res[i-1]++;
    for (auto &x: res) cerr << setw(2) << x << " ";
    cerr << endl;
  }
  */

  vector<ll> gcount(k+1);
  for (int i = 1; i <= k; i++) {
    for (int j = 1; i*j < k; j++) {
      gcount[i] += phi[j] * 2;
    }
    gcount[i]++;
  }

  ll total = 0;
  for (ll i = 1; i <= k; i++) {
    total += gcount[i] * i;
    cerr << gcount[i] << " ";
  }
  cerr << endl;
  cerr << total << endl;

  return 0;
}
