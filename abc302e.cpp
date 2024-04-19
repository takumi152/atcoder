#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<vector<int>> query(q, vector<int>(3));
  for (int i = 0; i < q; i++) {
    cin >> query[i][0] >> query[i][1];
    if (query[i][0] == 1) cin >> query[i][2];
  }

  vector<unordered_set<int>> graph(n+1);
  int isolated = n;
  vector<int> ans;
  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      if (graph[query[i][1]].empty()) isolated--;
      if (graph[query[i][2]].empty()) isolated--;
      graph[query[i][1]].insert(query[i][2]);
      graph[query[i][2]].insert(query[i][1]);
    }
    else if (query[i][0] == 2) {
      for (auto &v: graph[query[i][1]]) {
        graph[v].erase(query[i][1]);
        if (graph[v].empty()) isolated++;
      }
      if (!graph[query[i][1]].empty()) isolated++;
      graph[query[i][1]].clear();
    }

    ans.push_back(isolated);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
