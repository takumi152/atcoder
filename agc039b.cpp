#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<string> S(N);
  for (auto &x: S) cin >> x;

  int best = 0;
  for (int i = 0; i < N; i++) {
    queue<Pii> que;
    que.emplace(i, 1);
    vector<int> depth(N, 0);
    vector<bool> visited(N, false);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now.first]) continue;
      visited[now.first] = true;
      depth[now.first] = now.second;
      for (int j = 0; j < N; j++) {
        if (S[now.first][j] == '0') continue;
        else if (visited[j]) {
          if (depth[now.first]-1 != depth[j]) {
            cout << -1 << endl; // impossible
            return 0;
          }
        }
        else {
          que.emplace(j, now.second+1);
        }
      }
    }
    for (int j = 0; j < N; j++) {
      if (depth[j] > best) {
        best = depth[j];
      }
    }
  }

  cout << best << endl;

  return 0;
}
