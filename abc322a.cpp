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

  int n;
  cin >> n;
  string s;
  cin >> s;

  int ans = -1;
  for (int i = 0; i < n - 2; i++) {
    if (s[i] == 'A' && s[i+1] == 'B' && s[i+2] == 'C') {
      ans = i + 1;
      break;
    }
  }

  cout << ans << endl;

  return 0;
}
