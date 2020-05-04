#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<vector<int> > edge(n);
  for (int i = 1; i < n; i++) {
    int j;
    cin >> j;
    edge[i].push_back(j);
    edge[j].push_back(i);
  }

  vector<vector<int> > subtreeSize(n);
  for (int i = 0; i < n; i++) {
    subtreeSize[i] = vector<int>(edge[i].size());
  }
  vector<bool> visited(n);
  int first = 0;
  for (int i = 0; i < n; i++) {
    if (edge[i].size() == 1) {
      first = i;
      break;
    }
  }
  calcSubtreeSize(first, edge, subtreeSize, visited);

  vector<int> ans(n);
  for (int i = 0; i < n; i++) {
    for (auto &x: subtreeSize[i]) {
      ans[i] = max(ans[i], x);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
