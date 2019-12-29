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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  int r, s, p;
  cin >> r >> s >> p;
  string t;
  cin >> t;

  int score = 0;
  vector<bool> skipped(n);
  for (int i = 0; i < n; i++) {
    if (i >= k) {
      if (t[i] == t[i-k] && !skipped[i-k]) {
        skipped[i] = true;
        continue;
      }
    }
    if (t[i] == 'r') score += p;
    else if (t[i] == 's') score += r;
    else if (t[i] == 'p') score += s;
  }

  cout << score << endl;

  return 0;
}
