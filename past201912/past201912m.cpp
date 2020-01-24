#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<pair<double, double> > ab(n), cd(m);
  for (auto &x: ab) cin >> x.first >> x.second;
  for (auto &x: cd) cin >> x.first >> x.second;

  vector<int> idx = {0, 1, 2, 3, 4};
  bool improved = true;
  while (improved) {
    improved = false;
    unordered_set<int> selected;
    for (auto &x: idx) selected.insert(x);
    for (int k = 0; k < 5; k++) {
      pair<double, double> base = make_pair(0.0, 0.0);
      for (int i = 0; i < 5; i++) {
        if (i == k) continue;
        base.first += ab[idx[i]].first;
        base.second += ab[idx[i]].second;
      }
      double target = (base.second + ab[idx[k]].second) / (base.first + ab[idx[k]].first);
      int best = idx[k];
      for (int i = 0; i < n; i++) {
        if (selected.find(i) != selected.end()) continue;
        double score = (base.second + ab[i].second) / (base.first + ab[i].first);
        if (score > target) {
          target = score;
          best = i;
        }
      }
      if (best != idx[k]) {
        idx[k] = best;
        improved = true;
        break;
      }
    }
  }

  pair<double, double> after = make_pair(0.0, 0.0);
  for (int i = 0; i < 5; i++) {
    after.first += ab[idx[i]].first;
    after.second += ab[idx[i]].second;
  }
  double verybest = after.second / after.first;

  for (int l = 0; l < m; l++) {
    vector<int> idx = {0, 1, 2, 3};
    bool improved = true;
    while (improved) {
      improved = false;
      unordered_set<int> selected;
      for (auto &x: idx) selected.insert(x);
      for (int k = 0; k < 4; k++) {
        pair<double, double> base = cd[l];
        for (int i = 0; i < 4; i++) {
          if (i == k) continue;
          base.first += ab[idx[i]].first;
          base.second += ab[idx[i]].second;
        }
        double target = (base.second + ab[idx[k]].second) / (base.first + ab[idx[k]].first);
        int best = idx[k];
        for (int i = 0; i < n; i++) {
          if (selected.find(i) != selected.end()) continue;
          double score = (base.second + ab[i].second) / (base.first + ab[i].first);
          if (score > target) {
            target = score;
            best = i;
          }
        }
        if (best != idx[k]) {
          idx[k] = best;
          improved = true;
          break;
        }
      }
    }

    pair<double, double> after = cd[l];
    for (int i = 0; i < 4; i++) {
      after.first += ab[idx[i]].first;
      after.second += ab[idx[i]].second;
    }
    if (after.second / after.first > verybest) {
      verybest = after.second / after.first;
    }
  }

  cout << setprecision(16) << verybest << endl;

  return 0;
}
