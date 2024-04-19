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

  int k;
  cin >> k;

  ll ans = 0;
  int count = 0;
  queue<ll> que;
  for (int i = 1; i <= 9; i++) {
    que.push(i);
  }
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    count++;
    if (count == k) {
      ans = now;
      break;
    }
    for (int i = 0; i < now % 10; i++) {
      que.push(now * 10 + i);
    }
  }
  
  cout << ans << endl;

  return 0;
}
