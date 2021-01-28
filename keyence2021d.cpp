#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  vector<vector<bool> > ans(1 << n, vector<bool>(1 << n));
  ans[0][0] = true;
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < (1 << k); i++) {
      for (int j = 0; j < (1 << k); j++) {
        ans[i+(1<<k)][j] = ans[i][j];
        ans[i][j+(1<<k)] = ans[i][j];
        ans[i+(1<<k)][j+(1<<k)] = !ans[i][j];
      }
    }
  }

  // for (int i = 0; i < (1 << n); i++) {
  //   for (int j = i+1; j < (1 << n); j++) {
  //     int a = 0, b = 0;
  //     for (int k = 1; k < (1 << n); k++) {
  //       if (ans[k][i] == ans[k][j]) a++;
  //       else b++;
  //     }
  //     cerr << i << " " << j << " " << a << " " << b << endl;
  //   }
  // }

  cout << ans.size() - 1 << endl;
  for (int i = 1; i < (1 << n); i++) {
    for (int j = 0; j < (1 << n); j++) {
      if (ans[i][j]) cout << 'A';
      else cout << 'B';
    }
    cout << endl;
  }

  return 0;
}
