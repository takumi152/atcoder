#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stack>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, K;
  cin >> N >> K;
  vector<unordered_set<int> > edge;
  for (int i = 0; i < N; i++) edge.push_back(unordered_set<int>());
  for (int i = 0; i < N-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].insert(b-1); // zero index
    edge[b-1].insert(a-1); // zero index
  }

  if (N == 1) {
    cout << K << endl;
    return 0;
  }

  vector<ll> factorial(1, 1);
  for (ll i = 1; i < 2 * N; i++) factorial.push_back((factorial[i-1] * i) % MOD);
  int root = -1;
  for (int i = 0; i < N; i++) {
    if (edge[i].size() == 1) {
      root = i;
      break;
    }
  }
  ll ans = K;
  bool first = true;
  vector<bool> visited(N, false);
  stack<int> st;
  st.push(root);
  while(!st.empty()) {
    int now = st.top();
    st.pop();
    visited[now] = true;
    ll factor = K-2;
    if (first) {
      factor++;
      first = false;
    }
    for (int x : edge[now]) {
      if (visited[x]) continue;
      ans = (ans * factor) % MOD;
      factor--;
      st.push(x);
    }
  }

  cout << ans << endl;
  return 0;
}
