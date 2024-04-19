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

  string s;
  cin >> s;

  int n = s.size();

  int current_parity = 0;
  unordered_map<int, ll> parity_count;
  parity_count[current_parity]++;
  for (int i = 0; i < n; i++) {
    int num = s[i] - '0';
    current_parity ^= 1 << num;
    parity_count[current_parity]++;
  }

  ll ans = 0;
  for (auto &[_, c]: parity_count) {
    ans += c * (c - 1) / 2;
  }

  cout << ans << endl;

  return 0;
}
