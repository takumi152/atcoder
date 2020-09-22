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

  int m, k;
  cin >> m >> k;

  if ((m == 1 && k == 1) || (k >= (1 << m))) {
    cout << -1 << endl;
    return 0;
  }
  else if (m == 1 && k == 0) {
    cout << "0 0 1 1" << endl;
    return 0;
  }

  vector<int> ans;
  for (int i = 0; i < (1 << m); i++) {
    if (i != k) ans.push_back(i);
  }
  ans.push_back(k);
  for (int i = (1 << m) - 1; i >= 0; i--) {
    if (i != k) ans.push_back(i);
  }
  ans.push_back(k);

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
