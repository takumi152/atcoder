#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;

  vector<vector<int> > level(N, vector<int>(N, 0));
  int security = 1;
  bool updated = true;
  while(updated) {
    updated = false;
    vector<int> color(N, -1);
    for (int i = 0; i < N; i++) {
      for (int j = i+1; j < N; j++) {
        if (level[i][j] == 0 && ((color[i] == color[j] && color[i] == -1) || color[i] != color[j])) {
          updated = true;
          if (color[i] == 0) {
            color[j] = 1;
          }
          else {
            color[i] = 1;
            color[j] = 0;
          }
          level[i][j] = security;
          level[j][i] = security;
          stack<Pii> st;
          for (int k = 0; k < N; k++) {
            if (j != k) st.emplace(j, k);
          }
          while (!st.empty()) {
            auto now = st.top();
            st.pop();
            if (color[now.first] == color[now.second]) continue;
            if (level[now.first][now.second] != 0) continue;
            level[now.first][now.second] = security;
            level[now.second][now.first] = security;
            if (color[now.second] == -1) {
              if (color[now.first] == 0) color[now.second] = 1;
              else color[now.second] = 0;
              for (int k = 0; k < N; k++) {
                if (now.second != k) st.emplace(now.second, k);
              }
            }
          }
        }
      }
    }
    security++;
  }

  for (int i = 0; i < N-1; i++) {
    for (int j = i+1; j < N; j++) {
      cout << level[i][j];
      if (j < N-1) cout << " ";
      else cout << endl;
    }
  }

  return 0;
}
