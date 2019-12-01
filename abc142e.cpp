#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const int inf = (int) 1e9;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, M;
  cin >> N >> M;
  vector<int> a(M), b(M), c(M);
  for (int i = 0; i < M; i++) {
    cin >> a[i] >> b[i];
    for (int j = 0; j < b[i]; j++) {
      int box;
      cin >> box;
      c[i] |= (1 << (box - 1));
    }
  }

  vector<vector<int> > dp(M+1, vector<int>(1 << N, inf));
  for (int i = 0; i < M+1; i++) dp[i][0] = 0;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < (1 << N); j++) {
      dp[i+1][j] = min(dp[i][j], dp[i+1][j]);
      dp[i+1][j|c[i]] = min(dp[i+1][j|c[i]], min(dp[i][j|c[i]], dp[i][j] + a[i]));
    }
  }
  
  if (dp[M][(1<<N)-1] == inf) {
    cout << -1 << endl;
  }
  else {
    cout << dp[M][(1<<N)-1] << endl;
  }

  return 0;
}
