#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  map<Pii, int> count;

  for (int i = 1; i <= n; i++) {
    int first = i % 10;
    int t = i;
    while (t > 9) t /= 10;
    int last = t;
    count[Pii(first, last)]++;
  }

  int ans = 0;
  for (auto &x: count) ans += x.second * count[Pii(x.first.second, x.first.first)];

  cout << ans << endl;

  return 0;
}
