#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> p(n*n);
  for (auto &x: p) {
    cin >> x;
  }

  vector<vector<bool> > exist(n, vector<bool>(n, true));
  vector<vector<int> > dist(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      dist[i][j] = min(min(i, n-i-1), min(j, n-j-1));
    }
  }
  int ans = 0;
  for (auto &x: p) {
    ans += dist[(x-1)/n][(x-1)%n];
    exist[(x-1)/n][(x-1)%n] = false;
    queue<Pii> que;
    que.emplace((x-1)/n, (x-1)%n);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (now.first > 0) {
        if (!exist[now.first][now.second] && dist[now.first-1][now.second] > dist[now.first][now.second]) {
          dist[now.first-1][now.second] = dist[now.first][now.second];
          que.emplace(now.first-1, now.second);
        }
        else if (exist[now.first][now.second] && dist[now.first-1][now.second] > dist[now.first][now.second]+1) {
          dist[now.first-1][now.second] = dist[now.first][now.second]+1;
          que.emplace(now.first-1, now.second);
        }
      }
      if (now.first < n-1) {
        if (!exist[now.first][now.second] && dist[now.first+1][now.second] > dist[now.first][now.second]) {
          dist[now.first+1][now.second] = dist[now.first][now.second];
          que.emplace(now.first+1, now.second);
        }
        else if (exist[now.first][now.second] && dist[now.first+1][now.second] > dist[now.first][now.second]+1) {
          dist[now.first+1][now.second] = dist[now.first][now.second]+1;
          que.emplace(now.first+1, now.second);
        }
      }
      if (now.second > 0) {
        if (!exist[now.first][now.second] && dist[now.first][now.second-1] > dist[now.first][now.second]) {
          dist[now.first][now.second-1] = dist[now.first][now.second];
          que.emplace(now.first, now.second-1);
        }
        else if (exist[now.first][now.second] && dist[now.first][now.second-1] > dist[now.first][now.second]+1) {
          dist[now.first][now.second-1] = dist[now.first][now.second]+1;
          que.emplace(now.first, now.second-1);
        }
      }
      if (now.second < n-1) {
        if (!exist[now.first][now.second] && dist[now.first][now.second+1] > dist[now.first][now.second]) {
          dist[now.first][now.second+1] = dist[now.first][now.second];
          que.emplace(now.first, now.second+1);
        }
        else if (exist[now.first][now.second] && dist[now.first][now.second+1] > dist[now.first][now.second]+1) {
          dist[now.first][now.second+1] = dist[now.first][now.second]+1;
          que.emplace(now.first, now.second+1);
        }
      }
    }
  }

  cout << ans << endl;

  return 0;
}
