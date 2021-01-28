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

  string s;
  cin >> s;

  int n = s.size();

  reverse(s.begin(), s.end());

  vector<vector<int> > appearance(26);
  for (int i = 0; i < n; i++) {
    appearance[s[i]-'a'].push_back(i);
  }
  for (int i = 0; i < 26; i++) appearance[i].push_back(n); // sentinel

  vector<vector<int> > dp(1, vector<int>(26, -1));
  vector<vector<int> > prev(1, vector<int>(26, 0));
  for (int i = 1; ; i++) {
    dp.push_back(vector<int>(26, -1));
    prev.push_back(vector<int>(26, 0));

    for (int j = 0; j < 26; j++) {
      for (int k = 0; k < 26; k++) {
        int p = *(upper_bound(appearance[k].begin(), appearance[k].end(), dp[i-1][j]));
        if (p > dp[i][k]) {
          dp[i][k] = p;
          prev[i][k] = j;
        }
      }
    }

    // for (auto &x: dp[i]) cerr << setw(3) << x << " ";
    // cerr << endl;
    // for (auto &x: prev[i]) cerr << setw(3) << x << " ";
    // cerr << endl;

    bool finished = false;
    for (int j = 0; j < 26; j++) {
      if (dp[i][j] == n) {
        finished = true;
        break;
      }
    }
    if (finished) break;
  }

  string ans;
  int rem = dp.size()-1;
  int now = 0;
  for (int i = 0; i < 26; i++) {
    if (dp[rem][i] == n) {
      now = i;
      break;
    }
  }
  while (rem > 0) {
    ans.push_back('a'+now);
    now = prev[rem][now];
    rem--;
  }
  // reverse(ans.begin(), ans.end());

  cout << ans << endl;

  return 0;
}
