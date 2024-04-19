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

  ll ans = 0;
  ll gt_num = 0;
  for (int i = 0; i < n - 1; i++) {
    if (s[i] == '>') {
      gt_num++;
    }
    else {
      gt_num = 0;
    }
    ans += gt_num;
  }

  cout << ans << endl;

  return 0;
}
