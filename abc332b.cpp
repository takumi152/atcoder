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

  int k, g, m;
  cin >> k >> g >> m;

  int glass = 0;
  int mug = 0;
  for (int i = 0; i < k; i++) {
    if (glass == g) {
      glass = 0;
    }
    else if (mug == 0) {
      mug = m;
    }
    else {
      int to_glass = min(g - glass, mug);
      glass += to_glass;
      mug -= to_glass;
    }
  }

  cout << glass << " " << mug << endl;

  return 0;
}
