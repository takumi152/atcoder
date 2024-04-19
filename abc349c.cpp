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

  string s, t;
  cin >> s >> t;

  int n = s.size();
  int m = t.size();

  int substr_idx = 0;
  for (int i = 0; i < n; i++) {
    if (substr_idx >= 3) break;
    if (s[i] == t[substr_idx] + ('a' - 'A')) substr_idx++;
  }

  if (substr_idx >= 3 || (substr_idx == 2 && t[2] == 'X')) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
