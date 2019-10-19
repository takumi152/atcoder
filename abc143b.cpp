  #include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> d(n);
  for (auto &x: d) cin >> x;

  ll score = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      score += d[i] * d[j];
    }
  }

  cout << score << endl;

  return 0;
}
