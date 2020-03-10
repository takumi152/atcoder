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

  int sCount = 0;
  int tCount = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] == 'S') {
      sCount++;
    }
    else {
      if (sCount > 0) sCount--;
      else tCount++;
    }
  }

  int ans = sCount + tCount;

  cout << ans << endl;

  return 0;
}
