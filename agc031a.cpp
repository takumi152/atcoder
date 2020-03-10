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
  string s;
  cin >> s;

  unordered_map<char, ll> charCount;
  for (int i = 0; i < n; i++) charCount[s[i]]++;

  ll ans = 1;
  for (auto &x: charCount) {
    ans = (ans * (x.second + 1)) % mod;
  }
  ans--;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
