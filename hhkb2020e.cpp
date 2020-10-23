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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  int emptycell = 0;
  vector<vector<int> > count(h, vector<int>(w));
  vector<vector<bool> > visited_v(h, vector<bool>(w));
  vector<vector<bool> > visited_h(h, vector<bool>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '.') {
        emptycell++;
        if (!visited_v[i][j]) {
          vector<Pii> visited;
          visited.emplace_back(i, j);
          visited_v[i][j] = true;
          for (int k = i+1; k < h; k++) {
            if (s[k][j] == '.') {
              visited.emplace_back(k, j);
              visited_v[k][j] = true;
            }
            else break;
          }
          for (int k = i-1; k >= 0; k--) {
            if (s[k][j] == '.') {
              visited.emplace_back(k, j);
              visited_v[k][j] = true;
            }
            else break;
          }
          for (auto &x: visited) {
            count[x.first][x.second] += visited.size();
            if (visited_h[x.first][x.second]) count[x.first][x.second]--;
          }
        }
        if (!visited_h[i][j]) {
          vector<Pii> visited;
          visited.emplace_back(i, j);
          visited_h[i][j] = true;
          for (int k = j+1; k < w; k++) {
            if (s[i][k] == '.') {
              visited.emplace_back(i, k);
              visited_h[i][k] = true;
            }
            else break;
          }
          for (int k = j-1; k >= 0; k--) {
            if (s[i][k] == '.') {
              visited.emplace_back(i, k);
              visited_h[i][k] = true;
            }
            else break;
          }
          for (auto &x: visited) {
            count[x.first][x.second] += visited.size();
            if (visited_v[x.first][x.second]) count[x.first][x.second]--;
          }
        }
      }
    }
  }

  vector<ll> pow2(emptycell+1);
  pow2[0] = 1;
  for (int i = 1; i < emptycell+1; i++) pow2[i] = (pow2[i-1] * 2) % mod;
  vector<ll> pow2sum(emptycell+1);
  pow2sum[0] = 0;
  for (int i = 1; i < emptycell+1; i++) pow2sum[i] = (pow2sum[i-1] + pow2[emptycell-i]) % mod;

  ll ans = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      ans = (ans + pow2sum[count[i][j]]) % mod;
    }
  }

  cout << ans << endl;

  return 0;
}
