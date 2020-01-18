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
  vector<int> a(n);
  for (auto &x: a) {
    cin >> x;
    x--;
  }

  vector<bool> visited(n);
  visited[0] = true;
  int count = 0;
  int now = 0;
  while (true) {
    count++;
    now = a[now];
    if (now == 1) break;
    if (visited[now]) {
      cout << -1 << endl; // impossible
      return 0;
    }
    visited[now] = true;
  }

  cout << count << endl;

  return 0;
}
