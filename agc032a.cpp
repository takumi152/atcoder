#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> b(N);
  for (auto &x: b) cin >> x;

  for (int i = 0; i < N; i++) {
    if (b[i] > i+1) {
      cout << -1 << endl;
      return 0;
    }
  }

  stack<pair<vector<int>, vector<int> > > st;
  st.emplace(b, vector<int>());
  while (!st.empty()) {
    auto now = st.top();
    st.pop();
    if (now.first.size() == 0) {
      reverse(now.second.begin(), now.second.end());
      for (auto &x: now.second) cout << x << endl;
      return 0;
    }
    for (int i = 0; i < now.first.size(); i++) {
      if (now.first[i] == i+1) {
        auto newVec1 = now.first;
        newVec1.erase(newVec1.begin() + i);
        auto newVec2 = now.second;
        newVec2.push_back(i+1);
        st.emplace(newVec1, newVec2);
      }
    }
  }

  cout << -1 << endl;

  return 0;
}
