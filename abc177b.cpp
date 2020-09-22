#include <iostream>
#include <iomanip>
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

  string s, t;
  cin >> s >> t;

  int n = s.size();
  int m = t.size();

  int minimum = n;

  for (int i = 0; i < n - m + 1; i++) {
    int mismatch = 0;
    for (int j = 0; j < m; j++) {
      if (s[i+j] != t[j]) mismatch++;
    }
    if (mismatch < minimum) minimum = mismatch;
  }

  cout << minimum << endl;

  return 0;
}
