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

  int ans1 = (n+1)/2;
  for (int i = (n+1)/2; i < n; i++) {
    if (s[i] == s[(n+1)/2-1]) ans1++;
    else break;
  }
  reverse(s.begin(), s.end());
  int ans2 = (n+1)/2;
  for (int i = (n+1)/2; i < n; i++) {
    if (s[i] == s[(n+1)/2-1]) ans2++;
    else break;
  }
  int ans = min(ans1, ans2);

  cout << ans << endl;

  return 0;
}
