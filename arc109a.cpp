#include <iostream>
#include <iomanip>
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

  int a, b, x, y;
  cin >> a >> b >> x >> y;

  vector<bool> visited_a(101), visited_b(101);
  priority_queue<vector<int> , vector<vector<int> >, greater<vector<int> > > que;
  que.push(vector<int>({0, a, 0}));
  while (!que.empty()) {
    auto now = que.top();
    que.pop();
    if (now[2] == 0) {
      if (visited_a[now[1]]) continue;
      visited_a[now[1]] = true;
      if (!visited_b[now[1]]) que.push(vector<int>({now[0] + x, now[1], 1}));
      if (now[1] > 1) {
        if (!visited_a[now[1]-1]) que.push(vector<int>({now[0] + y, now[1] - 1, 0}));
        if (!visited_b[now[1]-1]) que.push(vector<int>({now[0] + x, now[1] - 1, 1}));
      }
      if (now[1] < 100) {
        if (!visited_a[now[1]+1]) que.push(vector<int>({now[0] + y, now[1] + 1, 0}));
      }
    }
    else {
      if (visited_b[now[1]]) continue;
      visited_b[now[1]] = true;
      if (now[1] == b) {
        cout << now[0] << endl;
        return 0;
      }
      if (!visited_a[now[1]]) que.push(vector<int>({now[0] + x, now[1], 0}));
      if (now[1] > 1) {
        if (!visited_b[now[1]-1]) que.push(vector<int>({now[0] + y, now[1] - 1, 1}));
      }
      if (now[1] < 100) {
        if (!visited_b[now[1]+1]) que.push(vector<int>({now[0] + y, now[1] + 1, 1}));
        if (!visited_a[now[1]+1]) que.push(vector<int>({now[0] + x, now[1] + 1, 0}));
      }
    }
  }

  return 0;
}
