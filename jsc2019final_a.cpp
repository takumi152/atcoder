#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, M;
  cin >> N >> M;
  vector<ll> A(N), B(M);
  for (auto &x: A) cin >> x;
  for (auto &y: B) cin >> y;

  unordered_set<int> combined;
  for (auto &x: A) {
    for (auto &y: B) {
      auto xy = x + y;
      if (combined.find(xy) != combined.end()) {
        vector<Pii> same;
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < M; j++) {
            if (A[i] + B[j] == xy) same.emplace_back(i, j);
            if (same.size() == 2) break;
          }
          if (same.size() == 2) break;
        }
        cout << same[0].first << " " << same[0].second << " " << same[1].first << " " << same[1].second << endl;
        return 0;
      }
      else combined.insert(xy);
    }
  }

  cout << -1 << endl; // not found
}
