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

  int n;
  cin >> n;
  vector<vector<int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  int diam_id_1;
  {
    vector<int> dist(n, 1000000007);
    queue<int> que;
    dist[0] = 0;
    que.push(0);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      for (auto &next: edge[now]) {
        if (dist[now] < dist[next]) {
          dist[next] = dist[now] + 1;
          que.push(next);
        }
      }
    }

    int furthest_id = 0;
    for (int i = 0; i < n; i++) {
      if (dist[i] > dist[furthest_id]) {
        furthest_id = i;
      }
    }

    diam_id_1 = furthest_id;
  }

  int diam_id_2;
  {
    vector<int> dist(n, 1000000007);
    queue<int> que;
    dist[diam_id_1] = 0;
    que.push(diam_id_1);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      for (auto &next: edge[now]) {
        if (dist[now] < dist[next]) {
          dist[next] = dist[now] + 1;
          que.push(next);
        }
      }
    }

    int furthest_id = 0;
    for (int i = 0; i < n; i++) {
      if (dist[i] > dist[furthest_id]) {
        furthest_id = i;
      }
    }

    diam_id_2 = furthest_id;
  }

  vector<int> center;
  {
    vector<bool> visited1(n);
    vector<bool> visited2(n);
    queue<int> que1;
    queue<int> que2;
    vector<int> que1_next;
    vector<int> que2_next;
    que1.push(diam_id_1);
    que2.push(diam_id_2);
    while (true) {
      bool found = false;
      while (!que1.empty()) {
        auto now = que1.front();
        que1.pop();
        visited1[now] = true;
        if (visited2[now]) {
          center.push_back(now);
          found = true;
        }
        for (auto &next: edge[now]) {
          if (!visited1[next]) que1_next.push_back(next);
        }
      }
      while (!que2.empty()) {
        auto now = que2.front();
        que2.pop();
        visited2[now] = true;
        if (visited1[now]) {
          center.push_back(now);
          found = true;
        }
        for (auto &next: edge[now]) {
          if (!visited2[next]) que2_next.push_back(next);
        }
      }
      if (found) break;
      for (auto &x: que1_next) que1.push(x);
      for (auto &x: que2_next) que2.push(x);
      que1_next.clear();
      que2_next.clear();
    }
  }

  int radius;
  {
    vector<int> dist(n, 1000000007);
    queue<int> que;
    dist[center[0]] = 0;
    que.push(center[0]);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      for (auto &next: edge[now]) {
        if (dist[now] < dist[next]) {
          dist[next] = dist[now] + 1;
          que.push(next);
        }
      }
    }

    int furthest_dist = 0;
    for (int i = 0; i < n; i++) {
      if (dist[i] > furthest_dist) {
        furthest_dist = dist[i];
      }
    }

    radius = furthest_dist;
  }

  vector<Pii> start;
  if ((int) center.size() == 1) {
    for (auto &adj: edge[center[0]]) start.emplace_back(center[0], adj);
  }
  else {
    start.emplace_back(center[0], center[1]);
    start.emplace_back(center[1], center[0]);
  }

  ll ans = 1;
  ll diam_vertex = 0;
  for (auto &[root, first_v]: start) {
    ll factor = 1;
    unordered_set<int> visited;
    queue<Pii> que;
    que.emplace(first_v, 1);
    while (!que.empty()) {
      auto [now, dist] = que.front();
      que.pop();
      visited.insert(now);
      if (dist == radius) {
        factor++;
        diam_vertex++;
      }
      for (auto &next: edge[now]) {
        if (next == root) continue;
        if (visited.find(next) == visited.end()) que.emplace(next, dist + 1);
      }
    }
    ans = (ans * factor) % mod;
  }

  ans -= diam_vertex + 1;
  if (ans < 0) ans += mod;

  cout << ans << endl;

  return 0;
}
