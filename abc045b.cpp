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

  string a, b, c;
  cin >> a >> b >> c;

  int pa = 0;
  int pb = 0;
  int pc = 0;
  char now = 'a';
  while (true) {
    if (now == 'a') {
      if (pa == a.length()) {
        cout << "A" << endl;
        return 0;
      }
      now = a[pa];
      pa++;
    }
    if (now == 'b') {
      if (pb == b.length()) {
        cout << "B" << endl;
        return 0;
      }
      now = b[pb];
      pb++;
    }
    if (now == 'c') {
      if (pc == c.length()) {
        cout << "C" << endl;
        return 0;
      }
      now = c[pc];
      pc++;
    }
  }

  return 0;
}
