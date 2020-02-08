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

  vector<int> t;
  int h = 0;
  for (int i = 1; i <= 999; i++) {
    h += i;
    t.push_back(h);
  }

  int diff = b - a;
  int ans = 0;
  for (int i = 0; i < 998; i++) {
    if (t[i+1] - t[i] == diff) {
      ans = t[i] - a;
      break;
    }
  }

  cout << ans << endl;

  return 0;
}
