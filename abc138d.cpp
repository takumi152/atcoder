#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, Q;
  cin >> N >> Q;
  vector<set<int> > edge(N);
  for (int i = 0; i < N-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].insert(b-1);
    edge[b-1].insert(a-1);
  }
  vector<pair<int, ll> > op;
  for (int i = 0; i < Q; i++) {
    int p;
    ll x;
    cin >> p >> x;
    op.emplace_back(p-1, x);
  }

  vector<ll> placement(N);
  for (auto &x: op) {
    placement[x.first] += x.second;
  }

  stack<pair<int, ll> > st;
  vector<ll> result(N);
  vector<bool> visited(N);
  st.emplace(0, 0);
  while(!st.empty()) {
    auto now = st.top();
    st.pop();
    if (visited[now.first]) continue;
    visited[now.first] = true;
    ll cost = now.second + placement[now.first];
    result[now.first] = cost;
    for (auto &y: edge[now.first]) {
      st.emplace(y, cost);
    }
  }

  for (int i = 0; i < N; i++) {
    cout << result[i];
    if (i < N-1) cout << " ";
    else cout << endl;
  }

  return 0;
}
