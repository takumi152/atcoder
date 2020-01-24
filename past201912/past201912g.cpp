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

  int n;
  cin >> n;
  vector<vector<int> > a(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      cin >> a[i][j];
      a[j][i] = a[i][j];
    }
  }

  int best = -1000000007;
  int lim = 1;
  for (int i = 0; i < n; i++) lim *= 3;
  for (int i = 0; i < lim; i++) {
    vector<int> assignment(n);
    int p = i;
    for (int j = 0; j < n; j++) {
      assignment[j] = p % 3;
      p /= 3;
    }
    int score = 0;
    for (int i = 0; i < n; i++) {
      for (int j = i+1; j < n; j++) {
        if (assignment[i] == assignment[j]) score += a[i][j];
      }
    }
    if (score > best) best = score;
  }

  cout << best << endl;

  return 0;
}
