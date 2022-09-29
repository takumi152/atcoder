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

  string s;
  cin >> s;

  int n = s.size();

  int first_non_a = -1;
  for (int i = 0; i < n; i++) {
    if (s[i] != 'a') {
      first_non_a = i;
      break;
    }
  }

  int last_non_a = -1;
  for (int i = n-1; i >= 0; i--) {
    if (s[i] != 'a') {
      last_non_a = i;
      break;
    }
  }

  if (first_non_a == -1) {
    cout << "Yes" << endl; // a only
    return 0;
  }

  if (first_non_a > (n - last_non_a) - 1) {
    cout << "No" << endl;
    return 0;
  }

  int p1 = first_non_a;
  int p2 = last_non_a;
  while (p1 <= p2) {
    if (s[p1] != s[p2]) {
      cout << "No" << endl;
      return 0;
    }
    p1++;
    p2--;
  }

  cout << "Yes" << endl;

  return 0;
}
