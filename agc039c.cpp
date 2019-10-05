// WA

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  string X;
  cin >> X;

  reverse(X.begin(), X.end());

  map<ll, ll> factor;
  for (int i = 3; i <= N; i+=2) {
    if (N % i == 0) factor[i] = 0;
  }
  vector<ll> pow2(N+2);
  pow2[0] = 1;
  for (int i = 1; i < N+2; i++) pow2[i] = (pow2[i-1] * 2) % MOD;

  ll base = 2*N;
  for (int i = 0; i < N; i++) {
    if (X[i] == '1') base = (base + pow2[i]*N*2) % MOD;
  }

  ll reduction = 0;
  for (auto &x: factor) {
    ll v = x.first;
    for (int i = 0; i < N / v; i++) {
      if (X[(N-N/v)+i] == '1') factor[v] = (factor[v] + pow2[i]) % MOD;
      //cout << (N-N/v)+i << endl;
    }
    factor[v]++;
    for (int i = N / v - 1; i >= 0; i--) {
      if (X[(N-N/v)+i] == '1') {
        if (X[(N-(N/v)*2)+i] == '1') break;
        else continue;
      }
      if (X[(N-(N/v)*2)+i] == '0') {
        if (X[(N-(N/v)*2)+i] == '1') continue;
        else {
          factor[v]--;
          break;
        }
      }
    }
    //cout << v << " " << factor[v] << " " << ((N*2) / v) << endl;
  }
  for (auto it1 = factor.rbegin(); it1 != factor.rend(); it1++) {
    for (auto it2 = it1; it2 != factor.rend(); it2++) {
      if (*it1 == *it2) continue;
      if ((*it1).first % (*it2).first == 0) {
        (*it2).second = ((*it2).second - (*it1).second) % MOD;
        if ((*it2).second < 0) (*it2).second += MOD;
      }
    }
  }
  for (auto &x: factor) {
    ll v = x.first;
    reduction = (reduction + (factor[v] * (N*2 - (N*2) / v))) % MOD;
    //cout << v << " " << factor[v] << " " << ((N*2) / v) << " " << (N*2 - (N*2) / v) << endl;
  }
  //cout << base << " " << reduction << endl;

  ll ans = (base - reduction) % MOD;
  if (ans < 0) ans += MOD;

  cout << ans << endl;

  return 0;
}
