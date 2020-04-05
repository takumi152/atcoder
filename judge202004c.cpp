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

  int a1, a2, a3;
  cin >> a1 >> a2 >> a3;

  int n = a1 + a2 + a3;
  int ans = 0;
  vector<int> perm(n);
  for (int i = 0; i < n; i++) perm[i] = i;
  do {
    bool good = true;
    vector<vector<int> > num(3);
    int p = 0;
    for (int j = 0; j < a1; j++) {
      num[0].push_back(perm[p]);
      p++;
    }
    for (int j = 0; j < a2; j++) {
      num[1].push_back(perm[p]);
      p++;
    }
    for (int j = 0; j < a3; j++) {
      num[2].push_back(perm[p]);
      p++;
    }
    for (int j = 0; j < a1; j++) {
      if (j > 0) {
        if (num[0][j] < num[0][j-1]) good = false;
      }
    }
    for (int j = 0; j < a2; j++) {
      if (j > 0) {
        if (num[1][j] < num[1][j-1]) good = false;
      }
      if (num[1][j] < num[0][j]) good = false;
    }
    for (int j = 0; j < a3; j++) {
      if (j > 0) {
        if (num[2][j] < num[2][j-1]) good = false;
      }
      if (num[2][j] < num[1][j]) good = false;
    }
    if (good) ans++;
  } while (next_permutation(perm.begin(), perm.end()));

  cout << ans << endl;

  return 0;
}
