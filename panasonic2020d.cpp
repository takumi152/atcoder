#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  vector<vector<int> > ans;
  queue<pair<vector<int>, int> > que;
  que.emplace(vector<int>(), 0);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (now.first.size() == n) {
      ans.push_back(now.first);
      continue;
    }
    for (int i = 0; i < now.second; i++) {
      vector<int> next = now.first;
      next.push_back(i);
      que.emplace(next, now.second);
    }
    vector<int> next = now.first;
    next.push_back(now.second);
    que.emplace(next, now.second+1);
  }

  sort(ans.begin(), ans.end());

  for (auto &x: ans) {
    for (auto &y: x) {
      char next = 'a' + y;
      cout << next;
    }
    cout << endl;
  }

  return 0;
}
