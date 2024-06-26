#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  string s;
  cin >> s;

  string ans = "";
  int qualified = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] == 'o' && qualified < k) {
      ans += 'o';
      qualified++;
    }
    else {
      ans += 'x';
    }
  }

  cout << ans << endl;

  return 0;
}
