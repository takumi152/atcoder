// WA

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
  vector<ll> killA(n), killB(m);
  for (auto &x: killA) cin >> x;
  for (auto &x: killB) cin >> x;

  ll totalDeathA = 0;
  ll totalDeathB = 0;
  for (int i = 0; i < n; i++) totalDeathB += killA[i];
  for (int i = 0; i < m; i++) totalDeathA += killB[i];

  vector<int> sameKillA;
  vector<int> sameKillB;
  sameKillA.push_back(1);
  for (int i = 1; i < n; i++) {
    if (killA[i] == killA[i-1]) sameKillA[sameKillA.size()-1]++;
    else sameKillA.emplace_back(1);
  }
  sameKillB.push_back(1);
  for (int i = 1; i < m; i++) {
    if (killB[i] == killB[i-1]) sameKillB[sameKillB.size()-1]++;
    else sameKillB.emplace_back(1);
  }

  int s = sameKillA.size();
  int t = sameKillB.size();

  vector<vector<ll> > deathCombA;
  vector<vector<ll> > deathCombB;
  for (int k = 0; k < s; k++) {
    vector<vector<ll> > dp(sameKillA[k]+1, vector<ll>(totalDeathA+1));
    dp[0][0] = 1;
    for (int i = 0; i < sameKillA[k]; i++) {
      dp[i+1][0] = 1;
      for (int j = 1; j <= totalDeathA; j++) {
        dp[i+1][j] = dp[i][j];
        if (j-(i+1) >= 0) dp[i+1][j] = (dp[i+1][j] + dp[i+1][j-(i+1)]) % mod;
      }
    }
    deathCombA.push_back(dp[sameKillA[k]]);
  }
  for (int k = 0; k < t; k++) {
    vector<vector<ll> > dp(sameKillB[k]+1, vector<ll>(totalDeathB+1));
    dp[0][0] = 1;
    for (int i = 0; i < sameKillB[k]; i++) {
      dp[i+1][0] = 1;
      for (int j = 1; j <= totalDeathB; j++) {
        dp[i+1][j] = dp[i][j];
        if (j-(i+1) >= 0) dp[i+1][j] = (dp[i+1][j] + dp[i+1][j-(i+1)]) % mod;
      }
    }
    deathCombB.push_back(dp[sameKillB[k]]);
  }

  vector<vector<ll> > dp;
  dp = vector<vector<ll> >(s+1, vector<ll>(totalDeathA+1));
  for (int i = 0; i < s; i++) {
    if (i <= 0) {
      dp[i+1][totalDeathA] = 1;
    }
    else {
      vector<ll> dp_sum(totalDeathA+2);
      dp_sum[0] = 0;
      for (int j = 0; j <= totalDeathA; j++) dp_sum[j+1] = (dp_sum[j] + dp[i][j]) % mod;
      for (int j = 0; j <= totalDeathA; j++) {
        dp[i+1][j] = (dp_sum[totalDeathA+1] - dp_sum[j]) % mod;
        if (dp[i+1][j] < 0) dp[i+1][j] += mod;
      }
    }
  }
  ll allDeathCombA = 0;
  for (int j = 0; j <= totalDeathA; j++) {
    allDeathCombA = (allDeathCombA + dp[s][j]) % mod;
  }
  for (int i = 0; i < s; i++) {
    for (int j = 0; j <= totalDeathA; j++) {
      allDeathCombA = (allDeathCombA + ((deathCombA[i][j] - 1) * dp[s][j]) % mod) % mod;
    }
  }
  dp = vector<vector<ll> >(t+1, vector<ll>(totalDeathB+1));
  for (int i = 0; i < t; i++) {
    if (i <= 0) {
      dp[i+1][totalDeathB] = 1;
    }
    else {
      vector<ll> dp_sum(totalDeathB+2);
      dp_sum[0] = 0;
      for (int j = 0; j <= totalDeathB; j++) dp_sum[j+1] = (dp_sum[j] + dp[i][j]) % mod;
      for (int j = 0; j <= totalDeathB; j++) {
        dp[i+1][j] = (dp_sum[totalDeathB+1] - dp_sum[j]) % mod;
        if (dp[i+1][j] < 0) dp[i+1][j] += mod;
      }
    }
  }
  ll allDeathCombB = 0;
  for (int j = 0; j <= totalDeathB; j++) {
    allDeathCombB = (allDeathCombB + dp[t][j]) % mod;
  }
  for (int i = 0; i < t; i++) {
    for (int j = 0; j <= totalDeathB; j++) {
      allDeathCombB = (allDeathCombB + ((deathCombB[i][j] - 1) * dp[t][j]) % mod) % mod;
    }
  }

  ll ans = (allDeathCombA * allDeathCombB) % mod;

  cout << ans << endl;

  return 0;
}
