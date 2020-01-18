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

  ll n, k, s;
  cin >> n >> k >> s;

  vector<ll> a(n);
  for (int i = 0; i < k; i++) {
    a[i] = s;
  }

  for (int i = k; i < n; i++) {
    if (s == 1000000000) a[i] = 999999999;
    else a[i] = 1000000000;
  }

  for (auto &x: a) cout << x << " ";
  cout << endl;

  return 0;
}
