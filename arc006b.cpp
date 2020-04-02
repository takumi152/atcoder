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

  int n, l;
  cin >> n >> l;
  vector<string> s(l+1);
  cin.ignore();
  for (auto &x: s) getline(cin, x);

  int now = 0;
  for (int i = 0; i < n; i++) {
    if (s[l][i*2] == 'o') {
      now = i;
      break;
    }
  }

  for (int i = l-1; i >= 0; i--) {
    if (now > 0) {
      if (s[i][now*2-1] == '-') {
        now--;
        continue;
      }
    }
    if (now < n-1) {
      if (s[i][now*2+1] == '-') {
        now++;
        continue;
      }
    }
  }

  cout << now+1 << endl;

  return 0;
}
