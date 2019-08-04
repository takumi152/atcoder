#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int A, B, C;
  cin >> A >> B >> C;

  cout << max(0, B + C - A) << endl;

  return 0;
}
