// WA

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

//const ll mod = 1000000007;
ll mod = 1000000007;

struct rerootingDP {
  int n;
  vector<vector<int> > edge;
  vector<int> deg;
  vector<unordered_map<int, int> > edgeid;
  vector<vector<ll> > edgeFactor;
  vector<vector<ll> > edgeFactorAccLeft;
  vector<vector<ll> > edgeFactorAccRight;
  vector<bool> fullvisited;
  vector<int> unvisited;

  rerootingDP(vector<vector<int> >& e) {
    n = e.size();
    edge = e;
    deg = vector<int>(n);
    for (int i = 0; i < n; i++) deg[i] = edge[i].size();
    edgeid = vector<unordered_map<int, int> >(n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < deg[i]; j++) edgeid[i][edge[i][j]] = j;
    }
    edgeFactor = vector<vector<ll> >(n);
    edgeFactorAccLeft = vector<vector<ll> >(n);
    edgeFactorAccRight = vector<vector<ll> >(n);
    for (int i = 0; i < n; i++) {
      edgeFactor[i] = vector<ll>(deg[i]);
      edgeFactorAccLeft[i] = vector<ll>(deg[i]+1);
      edgeFactorAccRight[i] = vector<ll>(deg[i]+1);
    }
    fullvisited = vector<bool>(n);
    unvisited = vector<int>(n);
    initDFS(-1, 0);
  }

  ll initDFS(int prev, int now) {
    if (prev == -1) fullvisited[now] = true;
    else unvisited[now] = prev;

    for (int i = 0; i < deg[now]; i++) {
      int adj = edge[now][i];
      if (adj == prev) continue;
      edgeFactor[now][i] = initDFS(now, adj);
    }

    if (prev == -1) {
      edgeFactorAccLeft[now][0] = 1;
      for (int i = 0; i < deg[now]; i++) {
        edgeFactorAccLeft[now][i+1] = (edgeFactorAccLeft[now][i] * (edgeFactor[now][i] + 1)) % mod;
      }
      edgeFactorAccRight[now][deg[now]] = 1;
      for (int i = deg[now]-1; i >= 0; i--) {
        edgeFactorAccRight[now][i] = (edgeFactorAccRight[now][i+1] * (edgeFactor[now][i] + 1)) % mod;
      }
    }
    else {
      edgeFactorAccLeft[now][0] = 1;
      for (int i = 0; i < edgeid[now][unvisited[now]]; i++) {
        edgeFactorAccLeft[now][i+1] = (edgeFactorAccLeft[now][i] * (edgeFactor[now][i] + 1)) % mod;
      }
      edgeFactorAccRight[now][deg[now]] = 1;
      for (int i = deg[now]-1; i > edgeid[now][unvisited[now]]; i--) {
        edgeFactorAccRight[now][i] = (edgeFactorAccRight[now][i+1] * (edgeFactor[now][i] + 1)) % mod;
      }
    }
    cerr << prev << " " << now << " n ";
    for (auto &x: edgeFactor[now]) cerr << x << " ";
    cerr << endl;
    cerr << prev << " " << now << " l ";
    for (auto &x: edgeFactorAccLeft[now]) cerr << x << " ";
    cerr << endl;
    cerr << prev << " " << now << " r ";
    for (auto &x: edgeFactorAccRight[now]) cerr << x << " ";
    cerr << endl;

    if (prev == -1) return (edgeFactorAccLeft[now][deg[now]]) % mod;
    else return (edgeFactorAccLeft[now][edgeid[now][unvisited[now]]] * edgeFactorAccRight[now][edgeid[now][unvisited[now]]]) % mod;
  }

  ll DFS(int prev, int now) {
    if (!fullvisited[now]) {
      if (prev != unvisited[now]) {
        edgeFactor[now][edgeid[now][unvisited[now]]] = DFS(now, unvisited[now]);
        for (int i = edgeid[now][unvisited[now]]; i < deg[now]-1; i++) {
          edgeFactorAccLeft[now][i+1] = (edgeFactorAccLeft[now][i] * (edgeFactor[now][i] + 1)) % mod;
        }
        for (int i = edgeid[now][unvisited[now]]; i > 0; i--) {
          edgeFactorAccRight[now][i-1] = (edgeFactorAccRight[now][i] * (edgeFactor[now][i] + 1)) % mod;
        }
        fullvisited[now] = true;
      }
    }

    if (prev == -1) return (edgeFactorAccLeft[now][deg[now]-1] * (edgeFactor[now][deg[now]-1] + 1)) % mod;
    else return (edgeFactorAccLeft[now][edgeid[now][unvisited[now]]] * edgeFactorAccRight[now][edgeid[now][unvisited[now]]]) % mod;
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  ll m;
  cin >> n >> m;
  vector<vector<int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1);
    edge[b-1].push_back(a-1);
  }

  if (n == 1) {
    cout << 1 << endl;
    return 0;
  }

  mod = m;

  rerootingDP rdp(edge);
  vector<ll> ans(n);
  for (int i = 0; i < n; i++) ans[i] = rdp.DFS(-1, i);

  for (int i = 0; i < n; i++) {
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
