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

  int n, m;
  cin >> n >> m;

  int singleTime = 1900 * m + 100 * (n - m);
  int expectedSub = 1 << m;
  int totalTime = singleTime * expectedSub;

  cout << totalTime << endl;

  return 0;
}
