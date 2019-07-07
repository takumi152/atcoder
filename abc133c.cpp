#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int L, R;
  cin >> L >> R;

  int left, right;
  if (L + 2019 <= R) {
    left = 0;
    right = 2019 + 2019;
  }
  else {
    left = L % 2019;
    right = R % 2019;
    if (left > right) right += 2019;
  }

  int minimum = 1000000007;
  for (int i = left; i < right; i++) {
    for (int j = i+1; j <= right; j++) {
      int x = (i * j) % 2019;
      if (x < minimum) minimum = x;
    }
  }

  cout << minimum << endl;
  return 0;
}
