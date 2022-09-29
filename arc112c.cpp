#include <iostream>
#include <iomanip>
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
  vector<int> parent(n-1);
  for (auto &x: parent) cin >> x;

  vector<vector<int> > children(n);
  for (int i = 0; i < n-1; i++) {
    children[parent[i]-1].push_back(i+1);
  }

  vector<bool> turnover(n);
  vector<int> score_loss(n);
  vector<int> score_gain(n);
  auto dfs1 = [&](auto self, int now) {
    if (children[now].empty()) {
      turnover[now] = true;
      score_loss[now] = 1;
      score_gain[now] = 0;
      return;
    }

    for (auto &next: children[now]) {
      self(self, next);
    }

    vector<vector<int> > vertex_take_order;
    for (auto &next: children[now]) vertex_take_order.push_back(vector<int>({!turnover[next], score_loss[next], score_gain[next]}));
    sort(vertex_take_order.begin(), vertex_take_order.end());

    turnover[now] = true;
    score_loss[now] = 1;
    score_gain[now] = 0;
    for (auto &x: vertex_take_order) {
      if (turnover[now]) {
        score_loss[now] += x[1];
        score_gain[now] += x[2];
      }
      else {
        score_loss[now] += x[2];
        score_gain[now] += x[1];
      }
      if (!x[0]) turnover[now] = !turnover[now];
    }
  };

  dfs1(dfs1, 0);

  // for (int i = 0; i < n; i++) cerr << i << " " << turnover[i] << " " << score_loss[i] << " " << score_gain[i] << endl;

  cout << score_loss[0] << endl;

  return 0;
}
