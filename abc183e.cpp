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

  vector<vector<ll> > dp_down(h+1, vector<ll>(w+1));
  vector<vector<ll> > dp_right(h+1, vector<ll>(w+1));
  vector<vector<ll> > dp_downright(h+1, vector<ll>(w+1));
  dp_down[1][1] = 1;
  dp_right[1][1] = 1;
  dp_downright[1][1] = 1;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (i == 0 && j == 0) continue;
      if (s[i][j] == '#') continue;
      ll base = dp_down[i][j+1] + dp_right[i+1][j] + dp_downright[i][j];
      dp_down[i+1][j+1] = (base + dp_down[i][j+1]) % mod;
      dp_right[i+1][j+1] = (base + dp_right[i+1][j]) % mod;
      dp_downright[i+1][j+1] = (base + dp_downright[i][j]) % mod;
    }
  }

  ll ans = (dp_down[h-1][w] + dp_right[h][w-1] + dp_downright[h-1][w-1]) % mod;

  cout << ans << endl;

  return 0;
}
