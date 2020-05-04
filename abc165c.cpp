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

  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<int> > abcd(q, vector<int>(4));
  for (auto &x: abcd) {
    cin >> x[0] >> x[1] >> x[2] >> x[3];
    x[0]--;
    x[1]--;
  }

  queue<vector<int> > que;
  for (int i = 0; i < m; i++) que.push(vector<int>({i}));
  ll ans = 0;
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if ((int) now.size() < n) {
      for (int i = now[now.size()-1]; i < m; i++) {
        auto next = now;
        next.push_back(i);
        que.push(next);
      }
    }
    else {
      ll score = 0;
      for (int i = 0; i < q; i++) {
        if (now[abcd[i][1]] - now[abcd[i][0]] == abcd[i][2]) score += abcd[i][3];
      }
      if (score > ans) ans = score;
    }
  }

  cout << ans << endl;

  return 0;
}
