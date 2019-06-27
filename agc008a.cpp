#include <iostream>
#include <cmath>

using namespace std;

typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int x, y;
  cin >> x >> y;

  int opCount = 0;
  if (abs(x) == abs(y)) {
    if (x == y) opCount = 0;
    else opCount = 1;
  }
  else if (abs(x) < abs(y)) {
    if (x < 0) opCount += 1;
    opCount += abs(y) - abs(x);
    if (y < 0) opCount += 1;
  }
  else { // abs(x) > abs(y)
    if (x > 0) opCount += 1;
    opCount += abs(x) - abs(y);
    if (y > 0) opCount += 1;
  }
  cout << opCount << endl;
  return 0;
}
