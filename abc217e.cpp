#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
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
  vector<Pii> query(q);
  for (auto &x: query) {
    cin >> x.first;
    if (x.first == 1) cin >> x.second;
  }

  queue<int> unsorted;
  priority_queue<int, vector<int>, greater<int> > sorted;

  vector<int> ans;
  for (int i = 0; i < q; i++) {
    if (query[i].first == 1) {
      unsorted.push(query[i].second);
    }
    else if (query[i].first == 2) {
      if (!sorted.empty()) {
        auto first = sorted.top();
        sorted.pop();
        ans.push_back(first);
      }
      else {
        auto first = unsorted.front();
        unsorted.pop();
        ans.push_back(first);
      }
    }
    else if (query[i].first == 3) {
      while (!unsorted.empty()) {
        auto next = unsorted.front();
        unsorted.pop();
        sorted.push(next);
      }
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
