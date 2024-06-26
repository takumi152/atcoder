#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  cout << (char) (n / 16 < 10 ? 48 + n / 16 : 55 + n / 16) << (char) (n % 16 < 10 ? 48 + n % 16 : 55 + n % 16) << endl;

  return 0;
}
