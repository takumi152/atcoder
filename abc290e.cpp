#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  unordered_map<int, vector<int>> num_positions;
  for (int i = 0; i < n; i++) {
    num_positions[a[i]].push_back(i);
  }

  ll ans = 0;
  for (ll i = 0; i < n; i++) ans += ((i + 1) / 2) * ((i + 2) / 2);
  for (auto &[_, positions]: num_positions) {
    vector<Pii> score_count;
    ll current_score = 0;
    for (auto &pos: positions) {
      int reduced_count = 0;
      while (!score_count.empty()) {
        auto [latest_pos, latest_count] = score_count.back();
        if (latest_pos >= n - pos - 1) {
          score_count.pop_back();
          current_score -= (latest_pos - (n - pos - 1)) * latest_count;
          reduced_count += latest_count;
        }
        else break;
      }
      if (reduced_count > 0) score_count.emplace_back(n - pos - 1, reduced_count);
      ans -= current_score;
      current_score += pos + 1;
      score_count.emplace_back(pos, 1);
    }
  }

  cout << ans << endl;

  return 0;
}
