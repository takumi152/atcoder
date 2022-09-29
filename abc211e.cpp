#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  unsigned long long int mask = 0xffffffffffffffff;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '#') mask &= ~(1ULL << (i * n + j));
    }
  }

  vector<unordered_set<unsigned long long int> > possible_pattern(k);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      possible_pattern[0].insert(0x0 | (1ULL << (i * n + j)));
    }
  }
  for (int c = 0; c < k-1; c++) {
    for (auto &x: possible_pattern[c]) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (x & (1ULL << (i * n + j))) {
            if (i > 0) {
              if (!(x & (1ULL << ((i-1) * n + j)))) possible_pattern[c+1].insert(x | (1ULL << ((i-1) * n + j)));
            }
            if (i < n-1) {
              if (!(x & (1ULL << ((i+1) * n + j)))) possible_pattern[c+1].insert(x | (1ULL << ((i+1) * n + j)));
            }
            if (j > 0) {
              if (!(x & (1ULL << (i * n + (j-1))))) possible_pattern[c+1].insert(x | (1ULL << (i * n + (j-1))));
            }
            if (j < n-1) {
              if (!(x & (1ULL << (i * n + (j+1))))) possible_pattern[c+1].insert(x | (1ULL << (i * n + (j+1))));
            }
          }
        }
      }
    }
  }

  int ans = 0;
  for (auto &x: possible_pattern[k-1]) {
    if ((x & mask) == x) {
      ans++;
    }
  }

  cout << ans << endl;

  return 0;
}
