// WIP

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
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  vector<int> adiff(n);
  vector<int> bdiff(n);
  for (int i = 0; i < n-1; i++) {
    adiff[i] = a[i] ^ a[i+1];
    bdiff[i] = b[i] ^ b[i+1];
  }
  adiff[n-1] = a[n-1] ^ a[0];
  bdiff[n-1] = b[n-1] ^ b[0];

  int firstFound = -1;
  int pa = 0;
  int pb = 0;
  bool looped = false;
  while (true) {
    if (adiff[pa] == bdiff[pb]) {
      pb++;
    }
    else {
      pb = 0;
      if (looped) break;
    }
    pa++;
    if (pa >= n) {
      pa = 0;
      looped = true;
    }
  }

  return 0;
}
