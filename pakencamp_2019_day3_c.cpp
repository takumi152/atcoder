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

  int n, m;
  cin >> n >> m;
  vector<vector<int> > a(n, vector<int>(m));
  for (auto &x: a) {
    for (auto &y: x) cin >> y;
  }

  ll best = 0;
  for (int j = 0; j < m; j++) {
    for (int k = j+1; k < m; k++) {
      ll score = 0;
      for (int i = 0; i < n; i++) {
        score += max(a[i][j], a[i][k]);
      }
      if (score > best) best = score;
    }
  }

  cout << best << endl;

  return 0;
}
