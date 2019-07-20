#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, D;
  cin >> N >> D;

  int r = N;
  int p = 0;
  while (r > 0) {
    r -= D * 2 + 1;
    p++;
  }

  cout << p << endl;

  return 0;
}
