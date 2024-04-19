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

  const string s = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";

  int n;
  cin >> n;

  cout << s.substr(0, 2 + n) << endl;

  return 0;
}
