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

  int N;
  cin >> N;

  int count = 0;
  for (int i = 1; i <= N; i++) {
    int x = i;
    int dig = 0;
    while (x > 0) {
      x /= 10;
      dig++;
    }
    if (dig % 2 == 1) count++;
  }

  cout << count << endl;

  return 0;
}
