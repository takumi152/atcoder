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
  vector<tuple<int, int, int>> fs(n);
  for (int i = 0; i < n; i++) {
    auto &[id, f, s] = fs[i];
    id = i;    
    cin >> f >> s;
  }

  sort(fs.rbegin(), fs.rend(), [](auto &a, auto &b){return get<2>(a) < get<2>(b);});

  unordered_map<int, vector<Pii>> tastes;
  for (auto &[id, f, s]: fs) {
    tastes[f].emplace_back(id, s);
  }
  for (auto &[f, s]: tastes) {
    sort(s.rbegin(), s.rend(), [](auto &a, auto &b){return a.second < b.second;});
  }

  int ans = 0;
  vector<int> flavor_start_pos(n);
  for (auto &[id1, f1, s1]: fs) {
    for (int i = flavor_start_pos[f1]; i < n; i++) {
      flavor_start_pos[f1] = i;
      auto &[id2, f2, s2] = fs[i];
      if (id1 == id2) continue;
      if (f1 == f2) continue;
      if (s1 + s2 > ans) {
        ans = s1 + s2;
      }
      break;
    }
    for (auto &[id2, s2]: tastes[f1]) {
      if (id1 == id2) continue;
      if (s1 + s2 / 2 > ans) {
        ans = s1 + s2 / 2;
      }
      break;
    }
  }

  cout << ans << endl;

  return 0;
}
