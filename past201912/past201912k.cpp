#include <iostream>
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
const ll inf = (ll) 9e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) {
    cin >> x;
    if (x > 0) x--; // zero index
  }
  int q;
  cin >> q;
  vector<Pii> ab(q);
  for (auto &x: ab) {
    cin >> x.first >> x.second;
    x.first--; // zero index
    x.second--; // zero index
  }

  int root = 0;
  for (int i = 0; i < n; i++) {
    if (p[i] == -1) {
      root = i;
      break;
    }
  }

  vector<vector<int> > child(n);
  for (int i = 0; i < n; i++) {
    if (p[i] == -1) continue;
    child[p[i]].push_back(i);
  }

  vector<int> label(n);
  vector<Pii> range(n);
  stack<Pii> st;
  st.emplace(root, 0);
  int visitCount = 0;
  while (!st.empty()) {
    auto now = st.top();
    st.pop();
    int node = now.first;
    int next = now.second;
    if (next == 0) {
      label[node] = visitCount;
      range[node].first = visitCount+1;
      visitCount++;
    }
    if (next >= child[node].size()) {
      range[node].second = visitCount;
    }
    else {
      st.emplace(node, next+1);
      st.emplace(child[node][next], 0);
    }
  }

  vector<string> ans(q);
  for (int i = 0; i < q; i++) {
    int target = label[ab[i].first];
    if (range[ab[i].second].first <= target && target < range[ab[i].second].second) ans[i] = "Yes";
    else ans[i] = "No";
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
