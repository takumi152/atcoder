#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <iterator>
#include <set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, Q;
  cin >> N >> Q;
  vector<ll> D(N);
  for (auto &x: D) cin >> x;
  vector<ll> T(Q), L(Q), R(Q);
  for (int i = 0; i < Q; i++) cin >> T[i] >> L[i] >> R[i];

  vector<ll> zeroTime(N+1);
  zeroTime[0] = 0;
  for (int i = 0; i < N; i++) zeroTime[i+1] = zeroTime[i] + D[i];
  vector<int> presence(Q);
  for (int i = 0; i < Q; i++) {
    auto itl = upper_bound(zeroTime.begin(), zeroTime.end(), T[i] - L[i]);
    auto itr = lower_bound(zeroTime.begin(), zeroTime.end(), T[i] - R[i]);
    presence[i] = distance(itr, itl);
  }

  for (auto &x: presence) cout << x << '\n';

  return 0;
}
