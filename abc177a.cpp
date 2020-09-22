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

  int d, t, s;
  cin >> d >> t >> s;

  int f = t * s;
  if (f >= d) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
