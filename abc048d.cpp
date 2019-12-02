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
  string ans;
  if (s[0] == s[n-1] && n % 2 == 0) ans = "First";
  else if (s[0] == s[n-1] && n % 2 == 1) ans = "Second";
  else if (s[0] != s[n-1] && n % 2 == 0) ans = "Second";
  else if (s[0] != s[n-1] && n % 2 == 1) ans = "First";

  cout << ans << endl;

  return 0;
}
