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

  int n;
  cin >> n;

  vector<vector<int> > ans;

  if (n % 2 == 0) {
    if (n == 2) {
      cout << "impossible" << endl;
      return 0;
    }
    for (int i = 0; i < n; i += 2) {
      ans.push_back(vector<int>({i+1, 2*n-1-i}));
    }

  }
  else {
    bool good = false;
    for (int k = 2; k < n; k++) {
      if (n % k == 0) {
        ans = vector<vector<int> >(k);
        int p = n*2-1;
        for (int i = 0; i < k; i++) {
          for (int j = 0; j < k; j++) {
            ans[(i+j)%k].push_back(p);
            p -= 2;
          }
        }
        int q = 1;
        while (q < p) {
          for (int i = 0; i < k; i++) {
            ans[i].push_back(q);
            ans[i].push_back(p);
            p -= 2;
            q += 2;
          }
        }
        good = true;
        break;
      }
    }
    if (!good) {
      cout << "impossible" << endl;
      return 0;
    }
  }

  cout << ans.size() << endl;
  for (auto &x: ans) {
    cout << x.size() << " ";
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
