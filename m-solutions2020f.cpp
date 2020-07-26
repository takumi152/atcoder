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

  int n;
  cin >> n;
  vector<ll> x(n), y(n);
  vector<char> u(n);
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> u[i];

  vector<vector<vector<int> > > horizontal(4, vector<vector<int> >(200001)),
                                vertical(4, vector<vector<int> >(200001)),
                                diagonal_positive(4, vector<vector<int> >(400001)),
                                diagonal_negative(4, vector<vector<int> >(400001));
  for (int i = 0; i < n; i++) {
    if (u[i] == 'L') {
      horizontal[0][y[i]].push_back(x[i]);
      diagonal_positive[0][x[i]-y[i]+200000].push_back(x[i]);
      diagonal_negative[0][x[i]+y[i]].push_back(x[i]);
    }
    else if (u[i] == 'R') {
      horizontal[1][y[i]].push_back(x[i]);
      diagonal_positive[1][x[i]-y[i]+200000].push_back(x[i]);
      diagonal_negative[1][x[i]+y[i]].push_back(x[i]);
    }
    else if (u[i] == 'U') {
      vertical[2][x[i]].push_back(y[i]);
      diagonal_positive[2][x[i]-y[i]+200000].push_back(x[i]);
      diagonal_negative[2][x[i]+y[i]].push_back(x[i]);
    }
    else if (u[i] == 'D') {
      vertical[3][x[i]].push_back(y[i]);
      diagonal_positive[3][x[i]-y[i]+200000].push_back(x[i]);
      diagonal_negative[3][x[i]+y[i]].push_back(x[i]);
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j <= 200000; j++) {
      sort(horizontal[i][j].begin(), horizontal[i][j].end());
      sort(vertical[i][j].begin(), vertical[i][j].end());
    }
    for (int j = 0; j <= 400000; j++) {
      sort(diagonal_positive[i][j].begin(), diagonal_positive[i][j].end());
      sort(diagonal_negative[i][j].begin(), diagonal_negative[i][j].end());
    }
  }

  int ans = 1000000007;
  for (int i = 0; i <= 200000; i++) {
    for (auto &px: horizontal[1][i]) {
      auto it = lower_bound(horizontal[0][i].begin(), horizontal[0][i].end(), px);
      if (it != horizontal[0][i].end()) {
        ans = min(ans, (*(it) - px) * 5);
      }
    }
    for (auto &py: vertical[2][i]) {
      auto it = lower_bound(vertical[3][i].begin(), vertical[3][i].end(), py);
      if (it != vertical[3][i].end()) {
        ans = min(ans, (*(it) - py) * 5);
      }
    }
  }
  for (int i = 0; i <= 400000; i++) {
    for (auto &px: diagonal_positive[2][i]) {
      auto it = lower_bound(diagonal_positive[0][i].begin(), diagonal_positive[0][i].end(), px);
      if (it != diagonal_positive[0][i].end()) {
        ans = min(ans, (*(it) - px) * 10);
      }
    }
    for (auto &px: diagonal_positive[1][i]) {
      auto it = lower_bound(diagonal_positive[3][i].begin(), diagonal_positive[3][i].end(), px);
      if (it != diagonal_positive[3][i].end()) {
        ans = min(ans, (*(it) - px) * 10);
      }
    }
    for (auto &px: diagonal_negative[1][i]) {
      auto it = lower_bound(diagonal_negative[2][i].begin(), diagonal_negative[2][i].end(), px);
      if (it != diagonal_negative[2][i].end()) {
        ans = min(ans, (*(it) - px) * 10);
      }
    }
    for (auto &px: diagonal_negative[3][i]) {
      auto it = lower_bound(diagonal_negative[0][i].begin(), diagonal_negative[0][i].end(), px);
      if (it != diagonal_negative[0][i].end()) {
        ans = min(ans, (*(it) - px) * 10);
      }
    }
  }

  if (ans == 1000000007) cout << "SAFE" << endl;
  else cout << ans << endl;

  return 0;
}
