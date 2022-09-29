#include <iostream>
#include <iomanip>
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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n;
  cin >> n;

  ll p = n;
  string ans = "";
  while (p > 0) {
    if (p % 2 == 0) {
      ans += 'B';
      p /= 2;
    }
    else {
      ans += 'A';
      p -= 1;
    }
  }

  reverse(ans.begin(), ans.end());

  cout << ans << endl;

  return 0;
}
