#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<ll> H(N);
  for (auto &x: H) cin >> x;

  H[0]--;
  for (int i = 1; i < N-1; i++) {
    if (H[i-1] < H[i]) H[i]--;
  }
  bool ok = true;
  for (int i = 0; i < N-1; i++) {
    if (H[i] > H[i+1]) ok = false;
  }
  if (ok) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
