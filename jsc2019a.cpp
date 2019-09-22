#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int M, D;
  cin >> M >> D;

  int ans = 0;
  for (int i = 1; i <= M; i++) {
    for (int j = 1; j <= D; j++) {
      int d1 = j % 10;
      int d10 = j / 10;
      if (d1 >= 2 && d10 >= 2 && d1 * d10 == i) ans++;
    }
  }

  cout << ans << endl;

  return 0;
}
