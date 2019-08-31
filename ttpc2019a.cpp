#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <iterator>
#include <set>
#include <cmath>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int A, B, T;
  cin >> A >> B >> T;

  int delta = B - A;
  int ans = B + delta;
  while (ans < T) ans += delta;

  cout << ans << endl;

  return 0;
}
