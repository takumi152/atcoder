#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

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

  vector<bool> visited(100000);
  int ans = 0;
  for (int i = 0; i < n; i++) {
    if (!visited[a[i]-1]) visited[a[i]-1] = true;
    else ans++;
  }

  cout << ans << endl;

  return 0;
}
