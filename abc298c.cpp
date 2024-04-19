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

  int n, q;
  cin >> n >> q;
  vector<vector<int>> query;
  for (int i = 0; i < q; i++) {
    int type;
    cin >> type;
    if (type == 1) {
      int i, j;
      cin >> i >> j;
      query.push_back({type, i, j});
    }
    else {
      int i;
      cin >> i;
      query.push_back({type, i});
    }
  }

  vector<multiset<int>> box(n+1);
  vector<set<int>> num_contained_at(200001);
  for (int i = 0; i < q; i++) {
    if (query[i][0] == 1) {
      box[query[i][2]].insert(query[i][1]);
      num_contained_at[query[i][1]].insert(query[i][2]);
    }
    else if (query[i][0] == 2) {
      for (auto &x: box[query[i][1]]) {
        cout << x << " ";
      }
      cout << endl;
    }
    else {
      for (auto &x: num_contained_at[query[i][1]]) {
        cout << x << " ";
      }
      cout << endl;
    }
  }

  return 0;
}
