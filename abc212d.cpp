#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int q;
  cin >> q;
  vector<vector<int> > query(q, vector<int>(2));
  for (int i = 0; i < q; i++) {
    cin >> query[i][0];
    if (query[i][0] != 3) cin >> query[i][1];
  }

  priority_queue<ll, vector<ll>, greater<ll> > que;
  ll global_added = 0;
  vector<ll> ans;
  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      que.push(query[i][1] - global_added);
    }
    else if (query[i][0] == 2) {
      global_added += query[i][1];
    }
    else {
      ans.push_back(que.top() + global_added);
      que.pop();
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
