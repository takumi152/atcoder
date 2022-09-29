#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<pair<double, double> > s(n), t(n);
  for (auto &x: s) cin >> x.first >> x.second;
  for (auto &x: t) cin >> x.first >> x.second;

  unordered_set<double> angle_set;
  angle_set.insert(0.0);
  for (int i = 1; i <= 50; i++) {
    for (int j = 1; j <= 50; j++) {
      for (int k = 1; k <= 100; k++) {
        if (i * i + j * j == k * k) {
          angle_set.insert(atan2(i, j));
        }
      }
    }
  }

  vector<double> angle_to_use;
  for (int i = 0; i < 4; i++) {
    for (auto &x: angle_set) angle_to_use.push_back(x + 3.14159265358979323846264 * (double) i / 2.0);
  }

  double dx = -s[0].first;
  double dy = -s[0].second;
  for (auto &x: s) {
    x.first += dx;
    x.second += dy;
  }

  for (auto &angle: angle_to_use) {
    auto s_now = s;
    for (auto &x: s_now) {
      double nx = x.first * cos(angle) - x.second * sin(angle);
      double ny = x.first * sin(angle) + x.second * cos(angle);
      x.first = nx;
      x.second = ny;
    }
    for (int k = 0; k < n; k++) {
      dx = t[k].first - s_now[0].first;
      dy = t[k].second - s_now[0].second;
      for (auto &x: s_now) {
        x.first += dx;
        x.second += dy;
      }
      bool all_match = true;
      for (int i = 0; i < n; i++) {
        bool match_found = false;
        for (int j = 0; j < n; j++) {
          if (abs(s_now[i].first - t[j].first) < 1e-6 && abs(s_now[i].second - t[j].second) < 1e-6) {
            match_found = true;
            break;
          }
        }
        if (!match_found) {
          all_match = false;
          break;
        }
      }
      if (all_match) {
        cout << "Yes" << endl;
        return 0;
      }
    }
  }

  cout << "No" << endl;

  return 0;
}
