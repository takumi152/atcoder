#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <queue>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int n = s.length();

  ll ans = 0;
  ll count = 0;
  ll lastCount = 0;
  char last = '<';
  for (int i = 0; i < n; i++) {
    if (s[i] == last) {
      ans += count;
      count++;
    }
    else {
      if (s[i] == '<') {
        ans += max(count, lastCount);
        count = 1;
      }
      else {
        lastCount = count;
        count = 1;
      }
      last = s[i];
    }
  }
  if (last == '>') {
    ans += max(count, lastCount);
  }
  else {
    ans += count;
  }

  cout << ans << endl;

  return 0;
}
