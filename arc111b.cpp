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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<unordered_set<int> > edge(400000);
  for (int i = 0; i < n; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].insert(b-1);
    edge[b-1].insert(a-1);
  }

  vector<bool> visited(400000);
  for (int i = 0; i < 400000; i++) {
    if (edge[i].size() == 1) {
      int now = i;
      while (edge[now].size() == 1) {
        int next = 0;
        for (auto &x: edge[now]) next = x;
        visited[now] = true;
        edge[now].erase(next);
        edge[next].erase(now);
        now = next;
      }
    }
  }

  for (int i = 0; i < 400000; i++) {
    if (edge[i].size() >= 2) visited[i] = true;
  }

  int ans = 0;
  for (int i = 0; i < 400000; i++) {
    if (visited[i]) ans++;
  }

  cout << ans << endl;

  return 0;
}
