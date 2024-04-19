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
  string s;
  cin >> s;

  int blank_used = 0;
  int logo_used = 0;
  int logo_max = 0;
  for (auto &x: s) {
    if (x == '0') {
      blank_used = 0;
      logo_used = 0;
    }
    else if (x == '1') {
      if (blank_used < m) blank_used++;
      else logo_used++;
    }
    else if (x == '2') {
      logo_used++;
    }
    logo_max = max(logo_max, logo_used);
  }

  cout << logo_max << endl;

  return 0;
}
