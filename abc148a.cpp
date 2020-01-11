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

  int a, b;
  cin >> a >> b;

  int x = min(a, b);
  int y = max(a, b);
  int z;
  if (x == 1 && y == 2) z = 3;
  if (x == 1 && y == 3) z = 2;
  if (x == 2 && y == 3) z = 1;

  cout << z << endl;

  return 0;
}
