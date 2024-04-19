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

  int n;
  cin >> n;
  vector<vector<int>> a(n);
  for (int i = 0; i < n; i++) {
    int c;
    cin >> c;
    a[i] = vector<int>(c);
    for (auto &x: a[i]) cin >> x;
  }
  int x;
  cin >> x;

  vector<int> winner;
  for (int i = 0; i < n; i++) {
    if (find_if(a[i].begin(), a[i].end(), [&](int y){return x == y;}) != a[i].end()) {
      winner.push_back(i);
    }
  }
  sort(winner.begin(), winner.end(), [&](auto &y, auto &z){
    if (a[y].size() != a[z].size()) return a[y].size() < a[z].size();
    else return y < z;
  });

  vector<int> ans;
  int min_bets = 999999;
  for (auto &i: winner) {
    if ((int) a[i].size() <= min_bets) {
      min_bets = a[i].size();
      ans.push_back(i);
    }
    else break;
  }

  cout << ans.size() << endl;
  for (auto &y: ans) cout << y + 1 << " ";
  cout << endl;

  return 0;
}
