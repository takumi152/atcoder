#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  unordered_map<ll, ll> numCount;
  for (int i = 0; i < n; i++) numCount[a[i]]++;

  ll base = 0;
  for (auto &x: numCount) base += (x.second * (x.second - 1)) / 2;

  vector<ll> ans(n);
  for (int i = 0; i < n; i++) {
    ans[i] = base - (numCount[a[i]] * (numCount[a[i]] - 1)) / 2 + ((numCount[a[i]] - 1) * (numCount[a[i]] - 2)) / 2;
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
