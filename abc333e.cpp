#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

struct Query {
  int t, x;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Query> queries(n);
  for (auto &query: queries) {
    cin >> query.t >> query.x;
    query.x--;
  }

  vector<int> ans;
  vector<stack<int>> potions(n);
  {
    int potion_num = 0;
    for (auto &query: queries) {
      if (query.t == 1) {
        potions[query.x].push(potion_num);
        ans.push_back(0);
        potion_num++;
      }
      if (query.t == 2) {
        if (potions[query.x].empty()) {
          cout << -1 << endl; // game over
          return 0;
        }
        int idx = potions[query.x].top();
        potions[query.x].pop();
        ans[idx] = 1;
      }
    }
  }

  int max_potion_num = 0;
  {
    int potion_num = 0;
    int potion_idx = 0;
    for (auto &query: queries) {
      if (query.t == 1) {
        if (ans[potion_idx] == 1) {
          potion_num++;
          max_potion_num = max(max_potion_num, potion_num);
        }
        potion_idx++;
      }
      if (query.t == 2) {
        potion_num--;
      }
    }
  }

  cout << max_potion_num << endl;
  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
