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

  vector<int> a(4);
  for (int i = 0; i < 4; i++) a[i] = s[i] - '0';

  for (int i = 0; i < (1 << 3); i++) {
    int total = a[0];
    for (int j = 0; j < 3; j++) {
      if (i & (1 << j)) total += a[j+1];
      else total -= a[j+1];
    }
    if (total == 7) {
      char op1 = (i & (1 << 0)) ? '+' : '-';
      char op2 = (i & (1 << 1)) ? '+' : '-';
      char op3 = (i & (1 << 2)) ? '+' : '-';
      cout << a[0] << op1 << a[1] << op2 << a[2] << op3 << a[3] << "=7" << endl;
      return 0;
    }
  }

  return 0;
}
