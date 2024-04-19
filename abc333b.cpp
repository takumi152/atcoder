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

  sort(s.begin(), s.end());
  sort(t.begin(), t.end());

  int s_len;
  if (s == "AB" || s == "BC" || s == "CD" || s == "DE" || s == "AE") s_len = 1;
  else s_len = 2;

  int t_len;
  if (t == "AB" || t == "BC" || t == "CD" || t == "DE" || t == "AE") t_len = 1;
  else t_len = 2;

  if (s_len == t_len) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
