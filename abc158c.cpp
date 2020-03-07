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

  int a, b;
  cin >> a >> b;

  for (int i = 1; i < 10000; i++) {
    int t8 = (i * 8) / 100;
    int t10 = (i * 10) / 100;
    if (t8 == a && t10 == b) {
      cout << i << endl;
      return 0;
    }
    else if (t8 > a || t10 > b) {
      cout << -1 << endl;
      return 0;
    }
  }

  return 0;
}
