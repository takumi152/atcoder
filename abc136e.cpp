#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

set<int> factorize(int n) {
  set<int> factor;
  int i = 1;
  while (i * i <= n) {
    if (n % i == 0) {
      factor.insert(i);
      factor.insert(n / i);
    }
    i++;
  }
  return factor;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, K;
  cin >> N >> K;
  vector<int> A(N);
  for (auto &x: A) cin >> x;

  int total = 0;
  for (int i = 0; i < N; i++) total += A[i];

  set<int> factor = factorize(total);

  int maximum = 0;
  for (int f: factor) {
    vector<int> r = A;
    for (auto &x: r) x %= f;
    sort(r.begin(), r.end());
    int remaining = K;
    int diff = 0;
    int left = 0, right = N-1;
    while (left < right) {
      if (diff <= 0) {
        diff += r[left];
        remaining -= max(0, diff);
        left++;
      }
      else if (diff > 0) {
        diff -= f - r[right];
        remaining -= max(0, -diff);
        right--;
      }
    }
    if (remaining >= 0) maximum = f;
  }

  cout << maximum << endl;

  return 0;
}
