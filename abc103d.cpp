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

  int n, m;
  cin >> n >> m;
  vector<Pii> ab(m);
  for (auto &x: ab) cin >> x.first >> x.second;

  priority_queue<Pii, vector<Pii>, greater<Pii> > a_que;
  priority_queue<int, vector<int>, greater<int> > b_que;
  for (int i = 0; i < m; i++) a_que.emplace(ab[i].first, i);
  int ans = 1;
  while (!a_que.empty()) {
    auto now = a_que.top();
    a_que.pop();
    if (!b_que.empty()) {
      if (now.first >= b_que.top()) {
        ans++;
        b_que = priority_queue<int, vector<int>, greater<int> >();
      }
    }
    b_que.push(ab[now.second].second);
  }

  cout << ans << endl;

  return 0;
}
