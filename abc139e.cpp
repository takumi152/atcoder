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
  vector<vector<int> > a(n, vector<int>(n-1));
  for (auto &x: a) {
    for (auto &y: x) {
      cin >> y;
      y--; // zero index
    }
  }

  vector<int> next(n);
  vector<int> waiting(n, -1);
  queue<int> que;
  for (int i = 0; i < n; i++) que.push(i);
  queue<int> pending;
  int ans = 0;
  while (true) {
    ans++;
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (next[now] == n-1) continue;
      waiting[now] = a[now][next[now]];
      if (waiting[a[now][next[now]]] == now) {
        waiting[a[now][next[now]]] = -1;
        next[a[now][next[now]]]++;
        if (next[a[now][next[now]]] < n-1) pending.push(a[now][next[now]]);
        waiting[now] = -1;
        next[now]++;
        if (next[now] < n-1) pending.push(now);
      }
    }
    while (!pending.empty()) {
      auto now = pending.front();
      pending.pop();
      que.push(now);
    }
    if (que.empty()) break;
  }

  bool good = true;
  for (int i = 0; i < n; i++) {
    if (next[i] != n-1) {
      good = false;
      break;
    }
  }

  if (good) {
    cout << ans << endl;
  }
  else {
    cout << -1 << endl;
  }

  return 0;
}
