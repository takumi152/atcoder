#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int q;
  cin >> q;
  vector<vector<int>> query;
  for (int i = 0; i < q; i++) {
    int type;
    cin >> type;
    if (type == 1) {
      int x;
      cin >> x;
      query.push_back({type, x});
    }
    else {
      query.push_back({type});
    }
  }

  vector<ll> p10 = {0, 1};
  for (int i = 2; i < 600002; i++) {
    p10.push_back((p10[i-1] * 10) % mod);
  }

  ll s = 1;
  queue<int> que;
  que.push(1);

  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      que.push(query[i][1]);
      s = (s * 10 + query[i][1]) % mod;
    }
    else if (query[i][0] == 2) {
      s = (s - p10[que.size()] * que.front()) % mod;
      if (s < 0) s += mod;
      que.pop();
    }
    else {
      cout << s << endl;
    }
  }

  return 0;
}
