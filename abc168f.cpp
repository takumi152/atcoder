#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > abc(n, vector<int>(3));
  for (int i = 0; i < n; i++) cin >> abc[i][0] >> abc[i][1] >> abc[i][2];
  vector<vector<int> > def(m, vector<int>(3));
  for (int i = 0; i < m; i++) cin >> def[i][0] >> def[i][1] >> def[i][2];

  sort(abc.begin(), abc.end(), [](auto &a, auto &b){return a[2] < b[2];});
  sort(def.begin(), def.end(), [](auto &a, auto &b){return a[0] < b[0];});

  set<int> pos_horizontal_set, pos_vertical_set;
  for (int i = 0; i < n; i++) {
    pos_horizontal_set.insert(abc[i][2]);
    pos_vertical_set.insert(abc[i][0]);
    pos_vertical_set.insert(abc[i][1]);
  }
  for (int i = 0; i < m; i++) {
    pos_vertical_set.insert(def[i][0]);
    pos_horizontal_set.insert(def[i][1]);
    pos_horizontal_set.insert(def[i][2]);
  }

  vector<int> pos_horizontal, pos_vertical;
  for (auto &x: pos_horizontal_set) pos_horizontal.push_back(x);
  for (auto &x: pos_vertical_set) pos_vertical.push_back(x);

  vector<vector<bool> > border_vertical(pos_horizontal.size(), vector<bool>(pos_vertical.size() + 1));
  vector<vector<bool> > border_horizontal(pos_vertical.size(), vector<bool>(pos_horizontal.size() + 1));
  for (int i = 0; i < n; i++) {
    int p_horizontal = distance(pos_horizontal.begin(), lower_bound(pos_horizontal.begin(), pos_horizontal.end(), abc[i][2]));
    int p_vertical_begin = distance(pos_vertical.begin(), upper_bound(pos_vertical.begin(), pos_vertical.end(), abc[i][0]));
    int p_vertical_end = distance(pos_vertical.begin(), upper_bound(pos_vertical.begin(), pos_vertical.end(), abc[i][1]));
    for (int j = p_vertical_begin; j < p_vertical_end; j++) border_vertical[p_horizontal][j] = true;
  }
  for (int i = 0; i < m; i++) {
    int p_vertical = distance(pos_vertical.begin(), lower_bound(pos_vertical.begin(), pos_vertical.end(), def[i][0]));
    int p_horizontal_begin = distance(pos_horizontal.begin(), upper_bound(pos_horizontal.begin(), pos_horizontal.end(), def[i][1]));
    int p_horizontal_end = distance(pos_horizontal.begin(), upper_bound(pos_horizontal.begin(), pos_horizontal.end(), def[i][2]));
    for (int j = p_horizontal_begin; j < p_horizontal_end; j++) border_horizontal[p_vertical][j] = true;
  }

  int p_origin_horizontal = distance(pos_horizontal.begin(), upper_bound(pos_horizontal.begin(), pos_horizontal.end(), 0));
  int p_origin_vertical = distance(pos_vertical.begin(), upper_bound(pos_vertical.begin(), pos_vertical.end(), 0));
  vector<vector<bool> > movable(pos_vertical.size() + 1, vector<bool>(pos_horizontal.size() + 1));
  queue<Pii> que;
  que.emplace(p_origin_vertical, p_origin_horizontal);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (movable[now.first][now.second]) continue;
    movable[now.first][now.second] = true;
    if (now.first > 0) {
      if (!border_horizontal[now.first-1][now.second] && !movable[now.first-1][now.second]) que.emplace(now.first-1, now.second);
    }
    if (now.first < (int) pos_vertical.size()) {
      if (!border_horizontal[now.first][now.second] && !movable[now.first+1][now.second]) que.emplace(now.first+1, now.second);
    }
    if (now.second > 0) {
      if (!border_vertical[now.second-1][now.first] && !movable[now.first][now.second-1]) que.emplace(now.first, now.second-1);
    }
    if (now.second < (int) pos_horizontal.size()) {
      if (!border_vertical[now.second][now.first] && !movable[now.first][now.second+1]) que.emplace(now.first, now.second+1);
    }
  }

  // for (int i = 0; i < (int) pos_vertical.size() + 1; i++) {
  //   for (int j = 0; j < (int) pos_horizontal.size() + 1; j++) {
  //     if (movable[i][j]) cerr << "o";
  //     else cerr << " ";
  //     if (j == (int) pos_horizontal.size()) break;
  //     if (border_vertical[j][i]) cerr << "|";
  //     else cerr << " ";
  //   }
  //   cerr << endl;
  //   if (i == (int) pos_vertical.size()) break;
  //   for (int j = 0; j < (int) pos_horizontal.size() + 1; j++) {
  //     if (border_horizontal[i][j]) cerr << "-";
  //     else cerr << " ";
  //     if (j == (int) pos_horizontal.size()) break;
  //     cerr << "+";
  //   }
  //   cerr << endl;
  // }

  ll ans = 0;
  for (int i = 0; i < (int) pos_vertical.size() + 1; i++) {
    for (int j = 0; j < (int) pos_horizontal.size() + 1; j++) {
      if (movable[i][j]) {
        if (i == 0 || i == (int) pos_vertical.size() || j == 0 || j == (int) pos_horizontal.size()) {
          cout << "INF" << endl;
          return 0;
        }
        ans += (ll) (pos_vertical[i] - pos_vertical[i-1]) * (pos_horizontal[j] - pos_horizontal[j-1]);
      }
    }
  }

  cout << ans << endl;

  return 0;
}
