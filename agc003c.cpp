#include <iostream>
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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> ai = a;
  sort(ai.begin(), ai.end());
  unordered_map<int, int> idx;
  for (int i = 0; i < n; i++) idx[ai[i]] = i;
  for (int i = 0; i < n; i++) a[i] = idx[a[i]];

  int ans = 0;
  for (int i = 0; i < n; i += 2) {
    if (a[i] % 2 != 0) ans++;
  }

  cout << ans << endl;

  return 0;
}
