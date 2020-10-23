#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <random>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  const int n = 13;

  mt19937 theMersenne(1);
  static vector<int> perm(26);
  for (int i = 0; i < 26; i++) perm[i] = i;

  vector<vector<int> > ans(n, vector<int>(n, -1));
  while (true) {
    ans = vector<vector<int> >(n, vector<int>(n, -1));
    vector<vector<bool> > used(26, vector<bool>(26));
    ans[0][0] = 0;
    bool done = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        shuffle(perm.begin(), perm.end(), theMersenne);
        for (int k = 0; k < 26; k++) {
          bool good = true;
          if (i > 0) {
            if (used[ans[i-1][j]][perm[k]]) good = false;
          }
          if (j > 0) {
            if (used[ans[i][j-1]][perm[k]]) good = false;
          }
          if (good) {
            ans[i][j] = perm[k];
            if (i > 0) {
              used[ans[i-1][j]][perm[k]] = true;
            }
            if (j > 0) {
              used[ans[i][j-1]][perm[k]] = true;
            }
            break;
          }
        }
        if (ans[i][j] == -1) {
          done = false;
          break;
        }
      }
      if (!done) break;
    }
    if (done) break;
  }

  cout << n << endl;
  for (auto &x: ans) {
    for (auto &y: x) cout << (char)('a' + y);
    cout << endl;
  }

  return 0;
}
