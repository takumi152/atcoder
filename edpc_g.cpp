#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, M;
  cin >> N >> M;
  vector<vector<int> > edge;
  for (int i = 0; i < N; i++) edge.push_back(vector<int>());
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1].push_back(b-1); // zero index
  }

  vector<int> longest(N, 0);
  vector<bool> visited(N, false);

  for (int i = 0; i < N; i++) {
    if (visited[i]) continue;
    stack<Pii> st;
    st.push(Pii(i, 0));
    int revCount = 0;
    while (!st.empty()) {
      Pii now = st.top();
      st.pop();
      visited[now.first] = true;
      if (now.second < edge[now.first].size()) {
        st.push(Pii(now.first, now.second+1));
        if (visited[edge[now.first][now.second]]) {
          longest[now.first] = max(longest[now.first], longest[edge[now.first][now.second]] + 1);
        }
        else {
          st.push(Pii(edge[now.first][now.second], 0));
          longest[now.first] = max(longest[now.first], revCount);
          revCount = 0;
        }
        continue;
      }
      else {
        longest[now.first] = max(longest[now.first], revCount);
        revCount = longest[now.first] + 1;
        continue;
      }
    }
  }

  cout << *max_element(longest.begin(), longest.end()) << endl;

  return 0;
}
