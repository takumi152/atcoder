#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  vector<unordered_set<int> > edge;
  for (int i = 0; i < n; i++) edge.push_back(unordered_set<int>());
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].insert(b-1); // zero index
  }
  int s, t;
  cin >> s >> t;
  s--; t--; // zero index

  queue<Pii> q;
  q.push(Pii(s, 0));
  const int unreachable = 1000000007;
  vector<int> minimum(n, unreachable);
  vector<int> current(n, 0);
  while (!q.empty()) {
    Pii now = q.front();
    q.pop();
    if (now.second % 3 == 0) {
      if (now.second < minimum[now.first]) minimum[now.first] = now.second;
      else continue;
    }
    for (int i : edge[now.first]) {
      if (now.second+1 != current[i]) {
        q.push(Pii(i, now.second+1));
        current[i] = now.second+1;
      }
    }
  }

  if (minimum[t] == unreachable) cout << -1 << endl;
  else cout << minimum[t]/3 << endl;
  return 0;
}
