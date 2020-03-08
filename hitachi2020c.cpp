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

  // check for star-shaped graph (no path with distance of 3)
  for (int i = 0; i < n; i++) {
    if (edge[i].size() == n-1) {
      for (int j = 1; j <= n; j++) cout << j << " ";
      cout << endl;
      return 0;
    }
  }

  vector<bool> parity(n);
  queue<Pii> que;
  vector<bool> visited(n);
  que.emplace(0, 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    parity[now.first] = (now.second % 2 == 0);
    for (auto &x: edge[now.first]) {
      if (!visited[x]) que.emplace(x, now.second + 1);
    }
  }

  int evenCount = 0;
  int oddCount = 0;
  for (int i = 0; i < n; i++) {
    if (parity[i]) evenCount++;
    else oddCount++;
  }

  vector<int> ans(n);
  int smallCount = min(evenCount, oddCount);
  if (smallCount < (n + 1) / 3) {
    int nextNum = 3;
    vector<bool> unused(n+2, true);
    if (evenCount < oddCount) {
      for (int i = 0; i < n; i++) {
        if (parity[i]) {
          ans[i] = nextNum;
          unused[nextNum] = false;
          nextNum += 3;
        }
      }
    }
    else {
      for (int i = 0; i < n; i++) {
        if (!parity[i]) {
          ans[i] = nextNum;
          unused[nextNum] = false;
          nextNum += 3;
        }
      }
    }
    nextNum = 1;
    for (int i = 0; i < n; i++) {
      if (ans[i] == 0) {
        ans[i] = nextNum;
        unused[nextNum] = false;
        while (!unused[nextNum]) nextNum++;
      }
    }
  }
  else {
    int nextNum = 1;
    for (int i = 0; i < n; i++) {
      if (parity[i]) {
        ans[i] = nextNum;
        nextNum += 3;
        if (nextNum > n) break;
      }
    }
    nextNum = 2;
    for (int i = 0; i < n; i++) {
      if (!parity[i]) {
        ans[i] = nextNum;
        nextNum += 3;
        if (nextNum > n) break;
      }
    }
    nextNum = 3;
    for (int i = 0; i < n; i++) {
      if (ans[i] == 0) {
        ans[i] = nextNum;
        nextNum += 3;
        if (nextNum > n) break;
      }
    }
  }

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
