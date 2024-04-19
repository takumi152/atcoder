#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  unordered_map<int, int> sock_count;
  for (auto &x: a) sock_count[x]++;

  int ans = 0;
  for (auto &[_, c]: sock_count) {
    ans += c / 2;
  }

  cout << ans << endl;

  return 0;
}
