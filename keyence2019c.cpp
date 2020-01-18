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

  int n;
  cin >> n;
  vector<ll> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<ll> diff(n);
  for (int i = 0; i < n; i++) {
    diff[i] = a[i] - b[i];
  }

  sort(diff.begin(), diff.end());

  ll insufficient = 0;
  int changed = 0;
  for (int i = 0; i < n; i++) {
    if (diff[i] < 0) {
      insufficient += -diff[i];
      changed++;
    }
    else break;
  }

  sort(diff.rbegin(), diff.rend());

  for (int i = 0; i < n; i++) {
    if (insufficient <= 0) break;
    if (diff[i] > 0) {
      insufficient -= diff[i];
      changed++;
    }
    else break;
  }

  if (insufficient <= 0) {
    cout << changed << endl;
  }
  else {
    cout << -1 << endl;
  }

  return 0;
}
