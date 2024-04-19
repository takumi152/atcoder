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

const ll mod = 998244353;

vector<ll> erathosthenes(ll n) {
  vector<bool> mask(n+1, true);
  mask[0] = false;
  mask[1] = false;
  for (int i = 2; i * i <= n; i++) {
    for (int j = i * i; j <= n; j += i) {
      mask[j] = false;
    }
  }
  vector<ll> prime = {};
  for (int i = 0; i <= n; i++) {
    if (mask[i]) prime.push_back(i);
  }
  return prime;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;

  const vector<ll> prime = erathosthenes(1000000);

  int ans = 0;
  int i, j, k;
  for (i = 0; i < (int) prime.size(); i++) {
    for (j = i + 1; j < (int) prime.size(); j++) {
      for (k = j + 1; k < (int) prime.size(); k++) {
        if (prime[i] * prime[i] * prime[j] * prime[k] * prime[k] <= n) {
          ans++;
        }
        else break;
      }
      if (k == j + 1) break;
    }
    if (j == i + 1) break;
  }

  cout << ans << endl;

  return 0;
}
