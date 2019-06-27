#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <stack>

using namespace std;

typedef pair<int, int> Pii;
typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  map<int, vector<Pii> > edge;
  for (int i = 0; i < N; i++) edge[i] = vector<Pii>();
  for (int i = 0; i < N-1; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    edge[a-1].push_back(Pii(b-1, c)); // zero index
    edge[b-1].push_back(Pii(a-1, c)); // zero index
  }
  int Q, K;
  cin >> Q >> K;
  K--; // zero index
  vector<Pii> query;
  for (int i = 0; i < Q; i++) {
    int x, y;
    cin >> x >> y;
    query.push_back(Pii(x-1, y-1)); // zero index
  }

  vector<ll> distFromK;
  vector<bool> visited;
  for (int i = 0; i < N; i++) {
    distFromK.push_back(0);
    visited.push_back(false);
  }
  stack<int> s;
  s.push(K);
  while (s.size() > 0) {
    int now = s.top();
    s.pop();
    for (auto& x : edge[now]) {
      if (!visited[x.first]) {
        distFromK[x.first] = distFromK[now] + x.second;
        s.push(x.first);
      }
    }
    visited[now] = true;
  }
  vector<ll> ans;
  for (int i = 0; i < Q; i++) {
    ans.push_back(distFromK[query[i].first] + distFromK[query[i].second]);
  }

  for (int i = 0; i < Q; i++) {
    cout << ans[i] << endl;
  }
  return 0;
}
