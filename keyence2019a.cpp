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

  vector<int> n(4);
  for (auto &x: n) cin >> x;

  sort(n.begin(), n.end());
  if (n[0] == 1 && n[1] == 4 && n[2] == 7 && n[3] == 9) {
    cout << "YES" << endl;
  }
  else {
    cout << "NO" << endl;
  }

  return 0;
}
