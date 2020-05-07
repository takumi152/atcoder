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

  string s;
  cin >> s;

  int n = s.length();
  int ans = 0;
  for (int i = 1; i * 2 < n; i++) {
    bool good = true;
    for (int j = 0; j < i; j++) {
      if (s[j] != s[j+i]) {
        good = false;
        break;
      }
    }
    if (good) ans = i * 2;
  }

  cout << ans << endl;

  return 0;
}
