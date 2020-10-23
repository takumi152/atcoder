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

  int n;
  string s;
  cin >> n >> s;

  int ans = 0;
  for (int i = 0; i < n; i++) {
    int a = 0;
    int t = 0;
    int c = 0;
    int g = 0;
    for (int j = i; j < n; j++) {
      if (s[j] == 'A') a++;
      else if (s[j] == 'T') t++;
      else if (s[j] == 'C') c++;
      else if (s[j] == 'G') g++;
      if (a == t && c == g) ans++;
    }
  }

  cout << ans << endl;

  return 0;
}
