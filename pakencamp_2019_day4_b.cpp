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

  int n;
  cin >> n;

  ll ans = 1;
  ll c = 1;
  for (int i = 0; i < n; i++) {
    c *= 5;
    ans += c;
  }

  cout << ans << endl;

  return 0;
}
