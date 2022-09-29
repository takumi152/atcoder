#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  int smallest_num = a[0];
  for (auto &x: a) smallest_num = min(smallest_num, x);

  priority_queue<int> que;
  for (auto &x: a) que.push(x);
  int ans = 0;
  while (que.size() > 1) {
    auto largest_num = que.top();
    que.pop();
    auto next_num = largest_num % smallest_num;
    ans++;
    if (next_num == 0) continue;
    smallest_num = min(smallest_num, next_num);
    que.push(next_num);
  }

  cout << ans << endl;

  return 0;
}
