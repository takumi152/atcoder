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

  string keyence = "keyence";
  int p = 0;
  for (int i = 0; i < 7; i++) {
    if (s[i] == keyence[p]) p++;
    else break;
  }
  for (int i = n-7+p; i < n; i++) {
    if (s[i] == keyence[p]) p++;
    else break;
  }

  if (p == 7) cout << "YES" << endl;
  else cout << "NO" << endl;

  return 0;
}
