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

  int n, m;
  cin >> n >> m;
  vector<vector<int>> x(m);
  for (int i = 0; i < m; i++) {
    int k;
    cin >> k;
    x[i] = vector<int>(k);
    for (int j = 0; j < k; j++) cin >> x[i][j];
  }

  vector<vector<bool>> participated_pair(n, vector<bool>(n));
  for (int i = 0; i < m; i++) {
    for (int j1 = 0; j1 < x[i].size(); j1++) {
      for (int j2 = j1 + 1; j2 < x[i].size(); j2++) {
        participated_pair[x[i][j1]-1][x[i][j2]-1] = true;
      }
    }
  }

  bool all_pair = true;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (!participated_pair[i][j]) all_pair = false;
    }
  }

  if (all_pair) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
