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
  vector<Pii> xl(n);
  for (auto &x: xl) cin >> x.first >> x.second;

  vector<Pii> ab(n);
  for (int i = 0; i < n; i++) ab[i] = Pii(xl[i].first - xl[i].second, xl[i].first + xl[i].second);

  priority_queue<Pii, vector<Pii>, greater<Pii> > que;
  for (int i = 0; i < n; i++) que.emplace(ab[i].second, ab[i].first);
  int last = -1000000007;
  int ans = 0;
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (last <= now.second) {
      ans++;
      last = now.first;
    }
  }

  cout << ans << endl;

  return 0;
}
