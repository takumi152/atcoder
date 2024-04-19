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

  int n, m;
  cin >> n >> m;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  sort(s.begin(), s.end());
  do {
    bool good = true;
    for (int i = 0; i < n - 1; i++) {
      int delta = 0;
      for (int j = 0; j < m; j++) {
        if (s[i][j] != s[i+1][j]) delta++;
      }
      if (delta != 1) {
        good = false;
        break;
      }
    }
    if (good) {
      cout << "Yes" << endl;
      return 0;
    }
  } while (next_permutation(s.begin(), s.end()));

  cout << "No" << endl;

  return 0;
}
