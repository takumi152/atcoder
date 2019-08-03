#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll A, B;
  cin >> A >> B;

  if (B <= A) cout << 1 << endl;
  else cout << 0 << endl;

  return 0;
}
