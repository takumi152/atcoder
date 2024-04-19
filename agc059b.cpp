#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<int> n(t);
  vector<vector<int>> c(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i];
    c[i] = vector<int>(n[i]);
    for (auto &x: c[i]) cin >> x;
  }

  vector<vector<int>> ans(t);
  for (int i = 0; i < t; i++) {
    sort(c[i].begin(), c[i].end());
    unordered_map<int, int> appear_count;
    for (int j = 0; j < n[i]; j++) appear_count[c[i][j]]++;

    vector<Pii> appear_count_sorted;
    for (auto &x: appear_count) appear_count_sorted.push_back(x);
    sort(appear_count_sorted.rbegin(), appear_count_sorted.rend(), [](auto &a, auto &b){return a.second < b.second;});

    if (appear_count_sorted[0].second == 1) {
      // impossible
      ans[i] = c[i];
      continue;
    }

    int fp = 0;
    int bp = appear_count_sorted.size() - 1;
    bool success = false;
    {
      ans[i].push_back(appear_count_sorted[fp].first);
      appear_count_sorted[fp].second--;
      while (fp < bp && appear_count_sorted[bp].second == 1) {
        if (appear_count_sorted[fp].second == 0 || (fp > 0 && appear_count_sorted[fp].second == 1)) {
          fp++;
          if (appear_count_sorted[fp].second == 1) break;
          ans[i].push_back(appear_count_sorted[fp].first);
          appear_count_sorted[fp].second--;
        }
        else {
          ans[i].push_back(appear_count_sorted[bp].first);
          bp--;
          ans[i].push_back(appear_count_sorted[fp].first);
          appear_count_sorted[fp].second--;
        }
      }
      if (fp == bp || appear_count_sorted[bp].second > 1) success = true;
    }
    if (!success) {
      // impossible
      ans[i] = c[i];
      continue;
    }

    if (fp < (int) appear_count_sorted.size()) {
      if ((appear_count_sorted[fp].second == 0 || (fp > 0 && appear_count_sorted[fp].second == 1)) && fp < bp) fp++;
      while (fp < (int) appear_count_sorted.size()) {
        if (appear_count_sorted[fp].second == 0 || (fp > 0 && appear_count_sorted[fp].second == 1)) {
          fp++;
          if (fp == (int) appear_count_sorted.size() || appear_count_sorted[fp].second == 1) {
            fp--;
            break;
          }
        }
        ans[i].push_back(appear_count_sorted[fp].first);
        appear_count_sorted[fp].second--;
      }
    }

    while (fp > 0) {
      ans[i].push_back(appear_count_sorted[fp].first);
      fp--;
    }
  }

  for (auto &x: ans) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
