#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <string>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007LL;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, M;
  cin >> N >> M;
  vector<string> S(M);
  for (auto &x: S) cin >> x;

  const int fizz = 3;
  const int buzz = 5;
  const int fizzbuzz = 15;
  const int none = 0;
  vector<int> si(M);
  for (int i = 0; i < M; i++) {
    if (S[i] == "Fizz") si[i] = fizz;
    else if (S[i] == "Buzz") si[i] = buzz;
    else if (S[i] == "FizzBuzz") si[i] = fizzbuzz;
  }
  si.push_back(none);

  // i: i桁目まで確認
  // j: j番目の発言まで行った
  // k: 3で割ったあまりがk
  vector<vector<vector<ll> > > dp(N+2, vector<vector<ll> >(M+2, vector<ll>(3)));
  for (int x = 1; x <= 9; x++) {
    if (x % 3 == 0) {
      if (si[0] == fizz) dp[1][1][x%3] += 1;
    }
    else if (x % 5 == 0) {
      if (si[0] == buzz) dp[1][1][x%3] += 1;
    }
    else dp[1][0][x%3] += 1;
  }

  for (int i = 2; i <= N; i++) {
    for (int j = 1; j <= M+1; j++) {
      for (int x = 0; x <= 9; x++) {
        if (x % 5 == 0) {
          if (si[j-1] != buzz && si[j-1] != fizzbuzz) continue;
          if (si[j-1] == buzz) {
            for (int k = 0; k < 3; k++) {
              if ((k+x)%3 != 0) dp[i][j][(k+x)%3] = (dp[i][j][(k+x)%3] + dp[i-1][j-1][k%3]) % MOD;
            }
          }
          else if (si[j-1] == fizzbuzz) {
            for (int k = 0; k < 3; k++) {
              if ((k+x)%3 == 0) dp[i][j][(k+x)%3] = (dp[i][j][(k+x)%3] + dp[i-1][j-1][k%3]) % MOD;
            }
          }
        }
        else {
          for (int k = 0; k < 3; k++) {
            if ((k+x)%3 == 0) {
              if (si[j-1] == fizz) dp[i][j][(k+x)%3] = (dp[i][j][(k+x)%3] + dp[i-1][j-1][k%3]) % MOD;
            }
            else {
              dp[i][j-1][(k+x)%3] = (dp[i][j-1][(k+x)%3] + dp[i-1][j-1][k%3]) % MOD;
            }
          }
        }
      }
    }
  }

  /*
  for (int i = 0; i <= N; i++) {
    for (int j = 0; j <= M+1; j++) {
      for (int k = 0; k < 3; k++) {
        cout << dp[i][j][k] << " ";
        if (k == 2) cout << "  ";
      }
      if (j < M+1) cout << " ";
      else cout << endl;
    }
    cout << endl;
  }
  */

  ll ans = 0;
  for (int k = 0; k < 3; k++) ans = (ans + dp[N][M][k]) % MOD;

  cout << ans << endl;

  return 0;
}
