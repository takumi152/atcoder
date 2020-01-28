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

  int h, n;
  cin >> h >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  for (auto &x: a) h -= x;
  if (h <= 0) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
