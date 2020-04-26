#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

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

  string s;
  cin >> s;

  int n = s.length();
  vector<int> t(n);
  for (int i = 0; i < n; i++) t[i] = s[i] - '0';

  vector<vector<int> > next(10, vector<int>(2019));
  for (int i = 1; i <= 9; i++) {
    vector<bool> visited(2019);
    for (int j = 0; j < 2019; j++) next[i][j] = j;
    for (int j = 0; j < 2019; j++) {
      if (!visited[j]) {
        int now = j;
        while (!visited[now]) {
          visited[now] = true;
          next[i][now] = (now * 10 + i) % 2019;
          now = next[i][now];
        }
      }
    }
  }


  vector<int> dp1(2019);
  vector<int> dp2(2019);
  ll ans = 0;
  for (int i = 0; i < n; i++) {
    if (i & 1) {
      dp2.assign(2019, 0);
      for (int j = 0; j < 2019; j++) {
        dp2[next[t[i]][j]] += dp1[j];
      }
      dp2[t[i]]++;
      ans += dp2[0];
    }
    else {
      dp1.assign(2019, 0);
      for (int j = 0; j < 2019; j++) {
        dp1[next[t[i]][j]] += dp2[j];
      }
      dp1[t[i]]++;
      ans += dp1[0];
    }
  }

  cout << ans << endl;

  return 0;
}
