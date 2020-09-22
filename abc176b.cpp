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

  string s;
  cin >> s;

  int total = 0;
  for (auto &x: s) total += x - '0';

  if (total % 9 == 0) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
