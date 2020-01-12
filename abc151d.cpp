#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;
const ll inf = 1000000007;

vector<vector<ll> > warshallFloyd(const vector<set<Pii> > &edge) {
  int n = edge.size();
  vector<vector<ll> > d(n, vector<ll>(n, inf));
  for (int i = 0; i < n; i++) {
    d[i][i] = 0;
    for (Pii x : edge[i]) {
      d[i][x.first] = x.second;
    }
  }
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
      }
    }
  }
  return d;
}
int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int h, w;
  cin >> h >> w;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  vector<set<Pii> > edge(h*w);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w-1; j++) {
      if (s[i][j] == '.' && s[i][j+1] == '.') {
        edge[i*w+j].emplace(i*w+j+1, 1);
        edge[i*w+j+1].emplace(i*w+j, 1);;
      }
    }
  }
  for (int i = 0; i < h-1; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '.' && s[i+1][j] == '.') {
        edge[i*w+j].emplace((i+1)*w+j, 1);
        edge[(i+1)*w+j].emplace(i*w+j, 1);
      }
    }
  }

  auto dist = warshallFloyd(edge);

  ll maximum = 0;
  for (int i = 0; i < h*w; i++) {
    for (int j = i+1; j < h*w; j++) {
      if (dist[i][j] < inf && dist[i][j] > maximum) maximum = dist[i][j];
    }
  }

  cout << maximum << endl;

  return 0;
}
