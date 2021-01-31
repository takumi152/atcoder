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

  int n, m;
  cin >> n >> m;
  vector<Pii> ab(m);
  for (auto &x: ab) cin >> x.first >> x.second;
  int k;
  cin >> k;
  vector<Pii> cd(k);
  for (auto &x: cd) cin >> x.first >> x.second;

  int ans = 0;
  for (int i = 0; i < (1 << k); i++) {
    vector<bool> placed(n);
    for (int j = 0; j < k; j++) {
      if (i & (1 << j)) placed[cd[j].first-1] = true;
      else placed[cd[j].second-1] = true;
    }
    int score = 0;
    for (int j = 0; j < m; j++) {
      if (placed[ab[j].first-1] && placed[ab[j].second-1]) score++;
    }
    ans = max(ans, score);
  }

  cout << ans << endl;

  return 0;
}
