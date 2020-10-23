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

  char s, t;
  cin >> s;
  cin >> t;

  if (s == 'Y') t += ('A' - 'a');

  cout << t << endl;

  return 0;
}
