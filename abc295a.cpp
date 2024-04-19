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

  int n;
  cin >> n;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  const vector<string> target = {"and", "not", "that", "the", "you"};

  bool good = false;
  for (auto &x: s) {
    for (auto &y: target) {
      if (x == y) good = true;
    }
  }

  if (good) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
