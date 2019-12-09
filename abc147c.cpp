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

int popcount(int x) {
  int a = 0;
  while (x > 0) {
    if (x & 1) a++;
    x >>= 1;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  vector<vector<int> > x(n), y(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    x[i] = vector<int>(a[i]);
    y[i] = vector<int>(a[i]);
    for (int j = 0; j < a[i]; j++) {
      cin >> x[i][j] >> y[i][j];
      x[i][j]--;
    }
  }

  int maximum = 0;
  for (int i = 0; i < (1 << n); i++) {
    int c = popcount(i);
    if (c <= maximum) continue;
    vector<bool> honest(n);
    int m = i;
    int p = 0;
    while (m > 0) {
      if (m & 1) honest[p] = true;
      m >>= 1;
      p++;
    }
    bool ok = true;
    for (int j = 0; j < n; j++) {
      if (honest[j]) {
        for (int k = 0; k < a[j]; k++) {
          bool isHonest = y[j][k];
          if (honest[x[j][k]] != isHonest) {
            ok = false;
            break;
          }
        }
      }
      if (!ok) break;
    }
    if (!ok) continue;
    else maximum = c;
  }

  cout << maximum << endl;

  return 0;
}
