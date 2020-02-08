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

  string s, t;
  cin >> s >> t;

  int n = s.length();
  int k = t.length();

  int latest = -1;
  for (int i = 0; i < n - k + 1; i++) {
    bool good = true;
    for (int j = 0; j < k; j++) {
      if (s[i+j] == '?') continue;
      else if (s[i+j] != t[j]) {
        good = false;
        break;
      }
    }
    if (good) latest = i;
  }

  if (latest == -1) {
    cout << "UNRESTORABLE" << endl;
    return 0;
  }

  for (int i = 0; i < k; i++) {
    s[latest+i] = t[i];
  }
  for (int i = 0; i < n; i++) {
    if (s[i] == '?') s[i] = 'a';
  }

  cout << s << endl;

  return 0;
}
