#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <deque>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<vector<int> > edge(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  vector<bool> visited(n);
  deque<int> ans;
  visited[0] = true;
  ans.push_back(0+1);

  int now = 0;
  while (true) {
    bool found = false;
    for (auto &next: edge[now]) {
      if (!visited[next]) {
        visited[next] = true;
        ans.push_front(next+1);
        now = next;
        found = true;
        break;
      }
    }
    if (!found) break;
  }

  now = 0;
  while (true) {
    bool found = false;
    for (auto &next: edge[now]) {
      if (!visited[next]) {
        visited[next] = true;
        ans.push_back(next+1);
        now = next;
        found = true;
        break;
      }
    }
    if (!found) break;
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
