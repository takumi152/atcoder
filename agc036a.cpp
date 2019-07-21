#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll S;
  cin >> S;

  ll uLimit = 1000000000;

  ll x1, y1, x2, y2, x3, y3;
  if (S >= 1000000000000000000LL) {
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = uLimit;
    x3 = uLimit;
    y3 = 0;
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << endl;
    return 0;
  }
  x1 = 0;
  y1 = 1;
  x2 = uLimit;
  y2 = 0;
  x3 = S % uLimit;
  y3 = S / uLimit + 1;
  /*
  cout << uLimit * y3 * 2 << endl;
  cout << y1 * x2 << endl;
  cout << (y3 - 1) * x3 << endl;
  cout << y3 * (uLimit - x3) << endl;
  cout << uLimit * y3 * 2 - y1 * x2 - (y3 - 1) * x3 - y3 * (uLimit - x3) << endl;
  */
  cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << endl;


  return 0;
}
