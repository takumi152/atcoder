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

  int n, k;
  cin >> n >> k;
  vector<int> l(n);
  for (auto &x: l) cin >> x;

  sort(l.rbegin(), l.rend());

  int ans = 0;
  for (int i = 0; i < k; i++) ans += l[i];

  cout << ans << endl;

  return 0;
}
