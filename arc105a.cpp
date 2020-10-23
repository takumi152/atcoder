#include <iostream>
#include <iomanip>
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

  int a, b, c, d;
  cin >> a >> b >> c >> d;

  int total = a + b + c + d;
  for (int i = 0; i < 16; i++) {
    int eaten = 0;
    if (i & 1) eaten += a;
    if (i & 2) eaten += b;
    if (i & 4) eaten += c;
    if (i & 8) eaten += d;
    if (total - eaten == eaten) {
      cout << "Yes" << endl;
      return 0;
    }
  }

  cout << "No" << endl;

  return 0;
}
