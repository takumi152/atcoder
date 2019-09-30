#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int gcd(int a, int b) {
  while (b > 0) {
    int rem = a % b;
    a = b;
    b = rem;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> A(N);
  for (auto &x: A) cin >> x;

  vector<int> left(N);
  vector<int> right(N);
  left[0] = A[0];
  for (int i = 1; i < N; i++) {
    left[i] = gcd(left[i-1], A[i]);
  }
  right[N-1] = A[N-1];
  for (int i = N-2; i >= 0; i--) {
    right[i] = gcd(right[i+1], A[i]);
  }
  int ans = max(left[N-2], right[1]);
  for (int i = 0; i < N-2; i++) {
    ans = max(ans, gcd(left[i], right[i+2]));
  }

  cout << ans << endl;

  return 0;
}
