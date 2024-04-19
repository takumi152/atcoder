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

  string s;
  cin >> s;

  int n = s.size();
  bool good = true;
  for (int i = 0; i < n - 1; i++) {
    if (s[i] <= s[i+1]) {
      good = false;
      break;
    }
  }

  if (good) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
