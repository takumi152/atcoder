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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > a(m);
  for (int i = 0; i < m; i++) {
    int k;
    cin >> k;
    a[i] = vector<int>(k);
    for (auto &x: a[i]) {
      cin >> x;
      x--;
    }
  }

  vector<Pii> position(n, Pii(-1, -1));
  vector<int> top_count(n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < (int) a[i].size(); j++) {
      if (position[a[i][j]].first == -1) position[a[i][j]].first = i;
      else position[a[i][j]].second = i;
      if (j == (int) a[i].size() - 1) top_count[a[i][j]]++;
    }
  }

  queue<int> que;
  for (int i = 0; i < n; i++) {
    if (top_count[i] == 2) que.push(i);
  }
  while (!que.empty()) {
    auto now = que.front();
    que.pop();

    a[position[now].first].pop_back();
    if (!a[position[now].first].empty()) {
      top_count[a[position[now].first][a[position[now].first].size()-1]]++;
      if (top_count[a[position[now].first][a[position[now].first].size()-1]] == 2) que.push(a[position[now].first][a[position[now].first].size()-1]);
    }
    a[position[now].second].pop_back();
    if (!a[position[now].second].empty()) {
      top_count[a[position[now].second][a[position[now].second].size()-1]]++;
      if (top_count[a[position[now].second][a[position[now].second].size()-1]] == 2) que.push(a[position[now].second][a[position[now].second].size()-1]);
    }
  }

  for (int i = 0; i < m; i++) {
    if (!a[i].empty()) {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
