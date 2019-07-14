// WIP

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, M;
  cin >> N >> M;
  vector<set<int> > edge(N, set<int>());
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].insert(b-1); // zero index
    edge[b-1].insert(a-1); // zero index
  }

  if (M % 2 == 1) {
    cout << -1 << endl; // impossible
    return 0;
  }

  vector<set<int> > dirEdge(N, set<int>());
  vector<bool> visited(N, false);
  for (int i = 0; i < N; i++) {
    if (visited[i]) continue;
    stack<Pii> st;
    st.push(Pii(i, 0));
    Pii last = Pii(-1, 0);
    while (!st.empty()) {
      Pii now = st.top();
      st.pop();
      if (edge[i].size() == 1) {

      }
      else if (edge[i].size() == 2) {

      }
      else if (edge[i].size() % 2 == 0) {

      }
    }
  }


  return 0;
}
