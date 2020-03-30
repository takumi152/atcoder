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

const ll mod = 1000000007;

ll modinv(ll x, ll m = mod) {
  ll b = m;
  ll u = 1;
  ll v = 0;
  ll tmp;
  while (b) {
    ll t = x / b;
    x -= t * b;
    tmp = x;
    x = b;
    b = tmp;
    u -= t * v;
    tmp = u;
    u = v;
    v = tmp;
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

int calcSubtreeSize(int now, vector<vector<int> >& edge, vector<vector<int> >& subtreeSize, vector<bool>& visited) {
  visited[now] = true;
  int deg = edge[now].size();
  for (int i = 0; i < deg; i++) {
    if (!visited[edge[now][i]]) subtreeSize[now][i] = calcSubtreeSize(edge[now][i], edge, subtreeSize, visited);
  }
  int n = edge.size() - 1;
  for (int i = 0; i < deg; i++) n -= subtreeSize[now][i];
  for (int i = 0; i < deg; i++) {
    if (subtreeSize[now][i] == 0) subtreeSize[now][i] = n;
  }
  return edge.size() - n;
}

ll calcFirstAns(int prev, int now, vector<vector<int> >& edge, vector<vector<int> >& subtreeSize, vector<bool>& visited, vector<ll>& factorial) {
  visited[now] = true;
  int deg = edge[now].size();
  ll vertexValue = 1;
  if (deg >= 3) {
    int subtreeTotal = 0;
    ll divisor = 1;
    for (int i = 0; i < deg; i++) {
      if (edge[now][i] == prev) continue;
      subtreeTotal += subtreeSize[now][i];
      divisor = (divisor * factorial[subtreeSize[now][i]]) % mod;
    }
    vertexValue = (factorial[subtreeTotal] * modinv(divisor)) % mod;
  }
  for (int i = 0; i < deg; i++) {
    if (!visited[edge[now][i]]) vertexValue = (vertexValue * calcFirstAns(now, edge[now][i], edge, subtreeSize, visited, factorial)) % mod;
  }
  return vertexValue;
}

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

  vector<unordered_map<int, int> > edgeid(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < (int)edge[i].size(); j++) {
      edgeid[i][edge[i][j]] = j;
    }
  }

  vector<vector<int> > subtreeSize(n);
  for (int i = 0; i < n; i++) subtreeSize[i] = vector<int>(edge[i].size());
  vector<bool> visited(n);

  calcSubtreeSize(0, edge, subtreeSize, visited);

  vector<ll> ans(n);
  int start = 0;
  for (int i = 0; i < n; i++) {
    if ((int)edge[i].size() == 1) {
      start = i;
      break;
    }
  }

  vector<ll> factorial(n+1);
  factorial[0] = 1;
  for (int i = 1; i <= n; i++) factorial[i] = factorial[i-1] * i;

  visited = vector<bool>(n);
  ans[start] = calcFirstAns(-1, start, edge, subtreeSize, visited, factorial);

  vector<ll> vertexFactor(n, 1);
  for (int i = 0; i < n; i++) {
    ll divisor = 1;
    for (auto &x: subtreeSize[i]) divisor = (divisor * factorial[x]) % mod;
    vertexFactor[i] = (factorial[n-1] * modinv(divisor)) % mod;
  }

  queue<Pii> que;
  que.emplace(-1, start);
  visited = vector<bool>(n);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.second]) continue;
    visited[now.second] = true;
    if (now.first >= 0) {
      ans[now.second] = ans[now.first];
      if ((int) edge[now.first].size() >= 3) {
        ans[now.second] = (ans[now.second] * factorial[n-1-subtreeSize[now.first][edgeid[now.first][now.second]]]) % mod;
        ans[now.second] = (ans[now.second] * modinv(factorial[n-1])) % mod;
        ans[now.second] = (ans[now.second] * factorial[subtreeSize[now.first][edgeid[now.first][now.second]]]) % mod;
      }
      else {
        ans[now.second] = (ans[now.second] * modinv(vertexFactor[now.first])) % mod;
      }
      if ((int) edge[now.second].size() >= 3) {
        ans[now.second] = (ans[now.second] * modinv(factorial[n-1-subtreeSize[now.second][edgeid[now.second][now.first]]])) % mod;
        ans[now.second] = (ans[now.second] * factorial[n-1]) % mod;
        ans[now.second] = (ans[now.second] * modinv(factorial[subtreeSize[now.second][edgeid[now.second][now.first]]])) % mod;
      }
      else {
        ans[now.second] = (ans[now.second] * vertexFactor[now.second]) % mod;
      }
    }
    for (auto &x: edge[now.second]) {
      if (!visited[x]) que.emplace(now.second, x);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
